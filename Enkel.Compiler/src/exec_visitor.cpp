#include "stdafx.h"
#include <cassert>
#include "enkel_runtime.h"
#include "exec_visitor.h"
#include "nodes.h"
#include "call_visitor.h"
#include "enkel_stl.h"
#include <iostream>

using namespace enkel::compiler;
using namespace std;

namespace enkel {
	namespace runtime {
		exec_visitor::exec_visitor(enkel_runtime &rt)
			: mRuntime(rt) {
		}

		exec_visitor::~exec_visitor() {
		}

		void exec_visitor::visit(module_node &node) {
			// Create a scope context for this module (just below global)
			mRuntime.scope_increase();

			for (auto &elem : node.get_elems()) {
				elem->accept(*this);
			}
		}

		void exec_visitor::visit(module_elem_node &node) {
			for (auto &elem : node.get_elems()) {
#if _DEBUG
				// All func_decl_nodes should've been removed at this point
				assert(!dynamic_cast<func_decl_node*>(elem.get()));
#endif
				elem->accept(*this);
			}
		}

		void exec_visitor::visit(bin_expr_node &node) {
			node.get_lhs()->accept(*this);
			variant_datatype val1 = dynamic_cast<expr_node*>(node.get_lhs().get())->get_val();
			node.get_rhs()->accept(*this);
			variant_datatype val2 = dynamic_cast<expr_node*>(node.get_rhs().get())->get_val();

			switch (node.get_op()) {
			case bin_expr_node::BIN_OP_PLUS:
				node.set_val(val1 + val2);
				break;
			case bin_expr_node::BIN_OP_EQUAL:
				node.set_val(val1 == val2);
				break;
			case bin_expr_node::BIN_OP_NEQUAL:
				node.set_val(val1 != val2);
				break;
			case bin_expr_node::BIN_OP_MUL:
				node.set_val(val1 * val2);
				break;
			case bin_expr_node::BIN_OP_LT: 
				node.set_val(val1 < val2);
				break;
			case bin_expr_node::BIN_OP_GT: 
				node.set_val(val1 > val2);
				break;
			case bin_expr_node::BIN_OP_LTEQ:
				node.set_val(val1 <= val2);
				break;
			case bin_expr_node::BIN_OP_GTEQ:
				node.set_val(val1 >= val2);
				break;
			default: ;
			}
		}

		void exec_visitor::visit(return_expr_node &node) {
			node.get_ret_expr()->accept(*this);
			node.set_val(dynamic_cast<expr_node*>(node.get_ret_expr().get())->get_val());
		}

		void exec_visitor::visit(var_decl_expr_node &node) {
			wstring varIdent = node.get_name();
			if (node.get_init_expr()) {
				node.get_init_expr()->accept(*this);
				auto existingVar = mRuntime.get_var(node.get_name());
				auto initVal = dynamic_cast<expr_node*>(node.get_init_expr().get())->get_val();

				if (existingVar
					&& existingVar->get_scope_level() < mRuntime.get_scope_level()) {
					// A var with this name exists, simply create a ref to it
					auto var = mRuntime.get_current_scope().add_var(existingVar->create_ref());
					var->set_data(initVal);
				}
				else {
					mRuntime.set_var(varIdent, initVal);
				}
			}
		}

		void exec_visitor::visit(var_expr_node &node) {
			node.set_val(mRuntime.get_var(node.get_name())->get_data());
		}

		void exec_visitor::visit(call_expr_node &node) {
			if (node.get_isstl()) {
				exec_stl_func(node);
			}
			else {
				func_decl_node& targetFunc = mRuntime.resolve_func(node.get_target_name());
				call_visitor caller(mRuntime, targetFunc);

				auto params = targetFunc.get_paramlist()->get_params();
				auto args = node.get_args();
				assert(params.size() == args.size());

				mRuntime.scope_increase();
				for (int i = 0; i < params.size(); i++) {
					var_expr_node *varExpr;
					if (params[i]->is_byref() && ((varExpr = dynamic_cast<var_expr_node*>(args[i].get())))) {
						auto ref = mRuntime.get_var(varExpr->get_name())->create_ref(params[i]->get_name());
						mRuntime.get_current_scope().add_var(ref);
					}
					else {
						args[i]->accept(*this);
						auto curArg = make_shared<rt_var>(mRuntime, params[i]->get_name());
						curArg->set_data(dynamic_cast<expr_node*>(args[i].get())->get_val());
						mRuntime.get_current_scope().add_var(curArg);
					}
				}

				node.accept(caller);
				if (caller.has_retval()) {
					node.set_val(caller.get_retval());
				}
				mRuntime.scope_decrease();
			}
		}

		void exec_visitor::visit(if_stmt_node &node) {
			if (node.get_cond()) {
				auto condExpr = dynamic_cast<expr_node*>(node.get_cond().get());
				condExpr->accept(*this);
				if (condExpr->get_val()) {
					mRuntime.scope_increase();
					node.get_true_block()->accept(*this);
					mRuntime.scope_decrease();
				}
				else {
					// There might not be an else block
					if (node.get_false_block()) {
						node.get_false_block()->accept(*this);
					}
				}
			}
			else {
				mRuntime.scope_increase();
				node.get_true_block()->accept(*this);
				mRuntime.scope_decrease();
			}
		}

		void exec_visitor::visit(block_node &node) {
			for (auto &elem : node.get_statements()) {
				elem->accept(*this);
			}
		}

		void exec_visitor::visit(func_decl_node &node) {
			node.get_body()->accept(*this);
		}

		void exec_visitor::visit(loop_stmt_node &node) {
			mRuntime.scope_increase();
			if(node.get_decl()) {
				node.get_decl()->accept(*this);
			}

			auto condExpr = dynamic_cast<expr_node*>(node.get_cond().get());
			// Evaluate initial condition
			condExpr->accept(*this);
			// Check if cond is a constant (simple 'loop' statement)
			auto condConstExpr = dynamic_cast<const_expr_node*>(condExpr);
			if (condConstExpr) {
				// Ensure it's an i32
				condConstExpr->get_val().change_to_i32();
				for (int i = 1; condConstExpr->get_val() >= i; i++) {
					node.get_body()->accept(*this);
				}
			}
			else {
				// If it's not a const expr, we need to evaluate each iteration
				while (condExpr->get_val()) {
					node.get_body()->accept(*this);
					if (node.get_incr()) {
						node.get_incr()->accept(*this);
					}
					condExpr->accept(*this);
				}
			}
			mRuntime.scope_decrease();
		}

		void exec_visitor::exec_stl_func(call_expr_node &node) {
			assert(node.get_isstl());

			enkel_stl::StlParamCont params;

			// If we need special parameters (e.g for print) we push them first
			if(node.get_target_name() == L"print") {
				params.push(make_shared<enkel_stl_param_wrapper>(&wcout));
			}

			// Push in reverse order
			for(auto it = node.get_args().rbegin(); it != node.get_args().rend();++it) {
				(*it)->accept(*this);
				// Take ownership of the variant
				params.push(make_shared<enkel_stl_param_wrapper>(move(dynamic_cast<expr_node*>((*it).get())->take_val())));
			}

			auto val = enkel_stl::exec_func(node.get_target_name(), params);

			// After executing the function we must return ownership of all variants back to
			// the argument nodes
			for (auto it = node.get_args().rbegin(); it != node.get_args().rend(); ++it) {
				dynamic_cast<expr_node*>((*it).get())->assign_val(move(params.top()->take_variant()));
				params.pop();
			}

			node.set_val(val);
		}

		bool exec_visitor::ensure_arg_count(vector<shared_ptr<base_node>> &args, int expectedCount) {
			bool isCorrect = args.size() == expectedCount;
			if (!isCorrect) {
				//TODO: runtime error
			}
			return isCorrect;
		}
	}
}

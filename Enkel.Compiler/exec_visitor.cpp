#include "stdafx.h"
#include "exec_visitor.h"
#include "nodes.h"
#include <cassert>
#include "enkel_runtime.h"

using namespace enkel::compiler;
using namespace std;

namespace enkel {
	namespace runtime {
		exec_visitor::exec_visitor(enkel_runtime &rt)
			: mRuntime(rt) {
		}

		exec_visitor::~exec_visitor() {
		}

		module_node& exec_visitor::visit(module_node &node) {
			// Create a scope context for this module (just below global)
			mRuntime.scope_increase();

			for (auto &elem : node.get_elems()) {
				elem->accept(*this);
			}

			return node;
		}

		module_elem_node& exec_visitor::visit(module_elem_node &node) {
			for (auto &elem : node.get_elems()) {
#if _DEBUG
				// All func_decl_nodes should've been removed at this point
				assert(!dynamic_cast<func_decl_node*>(elem.get()));
#endif
				elem->accept(*this);
			}

			return node;
		}

		block_node& exec_visitor::visit(block_node &node) {
			return node;
		}

		param_node& exec_visitor::visit(param_node &node) {
			return node;
		}

		assignment_stmt_node& exec_visitor::visit(assignment_stmt_node &node) {
			return node;
		}

		assign_expr_node& exec_visitor::visit(assign_expr_node &node) {
			return node;
		}

		bin_expr_node& exec_visitor::visit(bin_expr_node &node) {
			node.get_lhs()->accept(*this);
			variant_datatype val1 = dynamic_cast<expr_node*>(node.get_lhs().get())->get_val();
			node.get_rhs()->accept(*this);
			variant_datatype val2 = dynamic_cast<expr_node*>(node.get_rhs().get())->get_val();

			switch (node.get_op()) {
			case bin_expr_node::BIN_OP_PLUS:
				node.set_val(val1 + val2);
				break;
			default: ;
			}
			return node;
		}

		func_decl_node& exec_visitor::visit(func_decl_node &node) {
			return node;
		}

		func_node& exec_visitor::visit(func_node &node) {
			return node;
		}

		number_expr_node& exec_visitor::visit(number_expr_node &node) {
			return node;
		}

		return_expr_node& exec_visitor::visit(return_expr_node &node) {
			node.get_ret_expr()->accept(*this);
			node.set_val(dynamic_cast<expr_node*>(node.get_ret_expr().get())->get_val());
			return node;
		}

		var_decl_expr_node& exec_visitor::visit(var_decl_expr_node &node) {
			string varIdent = node.get_name();
			if (node.get_init_expr()) {
				node.get_init_expr()->accept(*this);
				auto initVal = dynamic_cast<expr_node*>(node.get_init_expr().get())->get_val();

				mRuntime.set_var(varIdent, initVal);
			}
			return node;
		}

		var_expr_node& exec_visitor::visit(var_expr_node &node) {
			node.set_val(mRuntime.get_var(node.get_name()));
			return node;
		}

		param_list_node& exec_visitor::visit(param_list_node &node) {
			return node;
		}

		call_expr_node& exec_visitor::visit(call_expr_node &node) {
			if(node.get_isstl()) {
				if(node.get_target_name() == "print") {
					ensure_arg_count(node.get_args(), 1);
					auto argExpr = dynamic_cast<expr_node*>(node.get_args()[0].get());
					argExpr->accept(*this);

					enkel_stl::print(mRuntime.get_ostream(), argExpr->get_val());
				}
			}
			return node;
		}

		bool exec_visitor::ensure_arg_count(vector<unique_ptr<base_node>> &args, int expectedCount) {
			bool isCorrect = args.size() == expectedCount;
			if(!isCorrect) {
				//TODO: runtime error
			}
			return isCorrect;
		}
	}
}

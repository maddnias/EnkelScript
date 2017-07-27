#include "stdafx.h"
#include "nodes.h"
#include "print_visitor.h"
#include <string>

using namespace std;

namespace enkel {
	namespace compiler {
		print_visitor::print_visitor(wostream &outStream)
			: mOutStream(outStream),
			mIndent(0),
			mNodeDepth(0) {
		}

		print_visitor::~print_visitor() {
		}

		void print_visitor::visit(module_node &node) {
			print(node.get_name(), L" {", NL);
			indent();
			for (auto &elem : node.get_elems()) {
				elem->accept(*this);
			}
			de_indent();
			print(L"}");
		}

		void print_visitor::visit(module_elem_node &node) {
			for (auto &elem : node.get_elems()) {
				elem->accept(*this);
			}
		}

		void print_visitor::visit(block_node &node) {
			for (auto &stmt : node.get_statements()) {
				stmt->accept(*this);
			}
		}

		void print_visitor::visit(param_node &node) {
			print(node.get_name());
		}

		void print_visitor::visit(assignment_stmt_node &node) {
			print(L" := ");
		}

		void print_visitor::visit(assign_expr_node &node) {
		}

		void print_visitor::visit(bin_expr_node &node) {
			node.get_lhs()->accept(*this);
			switch (node.get_op()) {
			case bin_expr_node::BIN_OP_PLUS:
				print(L" + ");
				break;

			case bin_expr_node::BIN_OP_EQUAL:
				print(L" = ");
				break;

			case bin_expr_node::BIN_OP_MUL:
				print(L" * ");
				break;

			case bin_expr_node::BIN_OP_LT: 
				print(L" < ");
				break;

			case bin_expr_node::BIN_OP_GT: 
				print(L" > ");
				break;

			case bin_expr_node::BIN_OP_LTEQ:
				print(L" <= ");
				break;

			case bin_expr_node::BIN_OP_GTEQ:
				print(L" >= ");
				break;

			default: ;
			}
			node.get_rhs()->accept(*this);
		}

		void print_visitor::visit(call_expr_node &node) {
			print(node.get_target_name(), L"(");
			int curIndent = reset_indent();
			int argCount = node.get_args().size();
			int counter = 0;
			mNodeDepth++;
			for (auto &arg : node.get_args()) {
				arg->accept(*this);
				if (++counter < argCount) {
					print(", ");
				}
			}
			mNodeDepth--;
			print(L")", mNodeDepth == 0 ? NL : L"");
			mIndent = curIndent;
		}

		void print_visitor::visit(func_decl_node &node) {
			print(NL);
			print(L"func ", node.get_name());
			int curIndent = reset_indent();
			node.get_paramlist()->accept(*this);
			mIndent = curIndent;
			indent();
			node.get_body()->accept(*this);
			de_indent();
			print(L"end", NL, NL);
		}

		void print_visitor::visit(func_node &node) {
		}

		void print_visitor::visit(const_expr_node &node) {
			// TODO: ugly
			// This is done to prevent consts like @NewLine to mess up the print
			wstring constName = enkel_stl::is_stl_const_val(node.get_val());
			if(!constName.empty()) {
				print("@", constName);
			}
			else {
				if(node.get_val().get_type() == runtime::variant_datatype::VAR_TYPE_STRING) {
					print(L"\"", node.get_val(), L"\"");
				}
				else {
					print(node.get_val());
				}
			}
		}

		void print_visitor::visit(return_expr_node &node) {
			print(L"return ");
			int curIndent = reset_indent();
			mNodeDepth++;
			node.get_ret_expr()->accept(*this);
			mNodeDepth--;
			mIndent = curIndent;
			print(NL);
		}

		void print_visitor::visit(var_decl_expr_node &node) {
			print(node.get_scope_decl(), node.get_name());
			if (node.get_init_expr()) {
				int curIndent = reset_indent();
				print(L" := ");
				mNodeDepth++;
				node.get_init_expr()->accept(*this);
				mNodeDepth--;
				mIndent = curIndent;
			}
			if (mNodeDepth == 0) {
				print(NL);
			}
		}

		void print_visitor::visit(var_expr_node &node) {
			print(node.get_name());
		}

		void print_visitor::visit(param_list_node &node) {
			print(L"(");
			int curIndent = reset_indent();
			int paramCount = node.get_params().size();
			int cur = 0;
			for (auto &param : node.get_params()) {
				if(param->is_byref()) {
					print("ref ");
				}
				param->accept(*this);
				if (++cur < paramCount) {
					print(L", ");
				}
			}
			mIndent = curIndent;
			print(L")", NL);
		}

		void print_visitor::visit(if_stmt_node &node) {
			if (node.get_cond()) {
				print(L"if ");
				int curIndent = reset_indent();
				node.get_cond()->accept(*this);
				print(NL);
				mIndent = curIndent;
			} else {
				print(L"else ", NL);
			}

			indent();
			node.get_true_block()->accept(*this);
			de_indent();

			if(node.get_false_block()) {
				if (dynamic_cast<if_stmt_node*>(node.get_false_block().get())->get_cond()) {
					print("else ");
				}
				node.get_false_block()->accept(*this);
			}
			else {
				print("end", NL);
			}
		}

		void print_visitor::visit(loop_stmt_node &node) {
			print(NL);
			// Determine what type of loop it is
			if (node.get_decl() && node.get_cond() && node.get_incr()) {
				print(L"for ");
				mNodeDepth++;
				int curIndent = reset_indent();
				node.get_decl()->accept(*this);
				print(L"; ");
				node.get_cond()->accept(*this);
				print(L"; ");
				node.get_incr()->accept(*this);
				mIndent = curIndent;
				mNodeDepth--;
				print(NL);
				indent();
				node.get_body()->accept(*this);
				de_indent();
				print(L"end", NL);
			}
			else {
				if(dynamic_cast<const_expr_node*>(node.get_cond().get())) {
					print(L"loop ");
					mNodeDepth++;
					int curIndent = reset_indent();
					node.get_cond()->accept(*this);
					mIndent = curIndent;
					mNodeDepth--;
					print(NL);
					indent();
					node.get_body()->accept(*this);
					de_indent();
					print(L"end", NL);
				} else {
					print(L"while ");
					mNodeDepth++;
					int curIndent = reset_indent();
					node.get_cond()->accept(*this);
					mIndent = curIndent;
					mNodeDepth--;
					print(NL);
					indent();
					node.get_body()->accept(*this);
					de_indent();
					print(L"end", NL, NL);
				}
			}
			print(NL);
		}

		int print_visitor::reset_indent() {
			int current = mIndent;
			mIndent = 0;
			return current;
		}

		void print_visitor::indent() {
			mIndent++;
		}

		void print_visitor::de_indent() {
			mIndent--;
		}

		wstring print_visitor::get_indent() const {
			wstring indent;
			for (int i = 0; i < mIndent; i++) {
				indent += L"\t";
			}
			return indent;
		}
	}
}

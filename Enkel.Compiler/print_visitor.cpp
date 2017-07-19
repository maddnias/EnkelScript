#include "stdafx.h"
#include "print_visitor.h"
#include "nodes.h"
#include <string>

using namespace std;

namespace enkel {
	namespace compiler {
		print_visitor::print_visitor(wostream &outStream)
			: mOutStream(outStream),
			  mIndent(0) {
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
			print(L" = ");
		}

		void print_visitor::visit(assign_expr_node &node) {
		}

		void print_visitor::visit(bin_expr_node &node) {
			node.get_lhs()->accept(*this);
			switch (node.get_op()) {
			case bin_expr_node::BIN_OP_PLUS:
				print(L" + ");
				break;
			}
			node.get_rhs()->accept(*this);
		}

		void print_visitor::visit(call_expr_node &node) {
		}


		void print_visitor::visit(func_decl_node &node) {
			print(L"func ", node.get_name());
			int curIndent = reset_indent();
			node.get_params()->accept(*this);
			mIndent = curIndent;
			indent();
			node.get_body()->accept(*this);
			de_indent();
			print(L"end", NL);
		}

		void print_visitor::visit(func_node &node) {
		}

		void print_visitor::visit(const_expr_node &node) {
			print(node.get_val());
		}

		void print_visitor::visit(return_expr_node &node) {
			print(L"return ");
			int curIndent = reset_indent();
			node.get_ret_expr()->accept(*this);
			mIndent = curIndent;
			print(NL);
		}

		void print_visitor::visit(var_decl_expr_node &node) {
			print(node.get_scope_decl(), node.get_name());
			if (node.get_init_expr()) {
				int curIndent = reset_indent();
				print(L" = ");
				node.get_init_expr()->accept(*this);
				mIndent = curIndent;
			}
			print(NL);
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
				param->accept(*this);
				if (++cur < paramCount) {
					print(L", ");
				}
			}
			mIndent = curIndent;
			print(L")", NL);
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

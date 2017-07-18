#include "stdafx.h"
#include "print_visitor.h"
#include "nodes.h"
#include <string>

using namespace std;

namespace enkel {
	namespace compiler {
		print_visitor::print_visitor(std::ostream &outStream)
			: mOutStream(outStream),
			  mIndent(0) {
		}

		print_visitor::~print_visitor() {
		}

		module_node& print_visitor::visit(module_node &node) {
			print(node.get_name(), " {", NL);
			indent();
			for (auto &elem : node.get_elems()) {
				elem->accept(*this);
			}
			de_indent();
			print("}");

			return node;
		}

		module_elem_node& print_visitor::visit(module_elem_node &node) {
			for (auto &elem : node.get_elems()) {
				elem->accept(*this);
			}

			return node;
		}

		block_node& print_visitor::visit(block_node &node) {
			for (auto &stmt : node.get_statements()) {
				stmt->accept(*this);
			}

			return node;
		}

		param_node& print_visitor::visit(param_node &node) {
			print(node.get_name());

			return node;
		}

		assignment_stmt_node& print_visitor::visit(assignment_stmt_node &node) {
			print(" = ");

			return node;
		}

		assign_expr_node& print_visitor::visit(assign_expr_node &node) {

			return node;
		}

		bin_expr_node& print_visitor::visit(bin_expr_node &node) {
			node.get_lhs()->accept(*this);
			switch (node.get_op()) {
			case bin_expr_node::BIN_OP_PLUS: 
				print(" + ");
				break;
			}
			node.get_rhs()->accept(*this);

			return node;
		}

		call_expr_node& print_visitor::visit(call_expr_node &node) {
			return node;
		}


		func_decl_node& print_visitor::visit(func_decl_node &node) {
			print("func ", node.get_name());
			int curIndent = reset_indent();
			node.get_params()->accept(*this);
			mIndent = curIndent;
			indent();
			node.get_body()->accept(*this);
			de_indent();
			print("end", NL);

			return node;
		}

		func_node& print_visitor::visit(func_node &node) {

			return node;
		}

		number_expr_node& print_visitor::visit(number_expr_node &node) {
			print(node.get_val());

			return node;
		}

		return_expr_node& print_visitor::visit(return_expr_node &node) {
			print("return ");
			int curIndent = reset_indent();
			node.get_ret_expr()->accept(*this);
			mIndent = curIndent;
			print(NL);

			return node;
		}

		var_decl_expr_node& print_visitor::visit(var_decl_expr_node &node) {
			print(node.get_scope_decl(), node.get_name());
			if (node.get_init_expr()) {
				int curIndent = reset_indent();
				print(" = ");
				node.get_init_expr()->accept(*this);
				mIndent = curIndent;
			}
			print(NL);

			return node;
		}

		var_expr_node& print_visitor::visit(var_expr_node &node) {
			print(node.get_name());

			return node;
		}

		param_list_node& print_visitor::visit(param_list_node &node) {
			print("(");
			int curIndent = reset_indent();
			int paramCount = node.get_params().size();
			int cur = 0;
			for (auto &param : node.get_params()) {
				param->accept(*this);
				if (++cur < paramCount) {
					print(", ");
				}
			}
			mIndent = curIndent;
			print(")", NL);

			return node;
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

		string print_visitor::get_indent() const {
			string indent;
			for (int i = 0; i < mIndent; i++) {
				indent += "\t";
			}
			return indent;
		}
	}
}

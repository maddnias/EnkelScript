#include "stdafx.h"
#include "func_transform_visitor.h"
#include "nodes.h"
#include "enkel_stl.h"

using namespace std;

namespace enkel {
	namespace compiler {
		func_transform_visitor::func_transform_visitor() {
		}

		func_transform_visitor::~func_transform_visitor() {
		}

		module_node& func_transform_visitor::visit(module_node &node) {
			for (auto &elem : node.get_elems()) {
				elem->accept(*this);
			}

			return node;
		}

		module_elem_node& func_transform_visitor::visit(module_elem_node &node) {
			vector<int> transferredIndices;
			int idx = 0;
			for (auto &elem : node.get_elems()) {
				// Make sure it's a func_decl_node before transferring ownership
				if (dynamic_cast<func_decl_node*>(elem.get())) {
					// Transfer ownership 
					mFuncDecls.push_back(unique_ptr<func_decl_node>(
						dynamic_cast<func_decl_node*>(elem.release())));
					transferredIndices.push_back(idx);
				}
				idx++;
			}

			// Delete the now NULL func_decl_nodes from the module_elem_node 
			// mainly to prevent potential issues if additional visitors are used after func_extract_visitor
			int removedCount = 0;
			for (auto &transferredIdx : transferredIndices) {
				node.remove_elem(transferredIdx - removedCount++);
			}

			return node;
		}

		block_node& func_transform_visitor::visit(block_node &node) {
			return node;
		}

		param_node& func_transform_visitor::visit(param_node &node) {
			return node;
		}

		assignment_stmt_node& func_transform_visitor::visit(assignment_stmt_node &node) {
			return node;
		}

		assign_expr_node& func_transform_visitor::visit(assign_expr_node &node) {
			return node;
		}

		bin_expr_node& func_transform_visitor::visit(bin_expr_node &node) {
			return node;
		}

		func_decl_node& func_transform_visitor::visit(func_decl_node &node) {
			return node;
		}

		func_node& func_transform_visitor::visit(func_node &node) {
			return node;
		}

		number_expr_node& func_transform_visitor::visit(number_expr_node &node) {
			return node;
		}

		return_expr_node& func_transform_visitor::visit(return_expr_node &node) {
			return node;
		}

		var_decl_expr_node& func_transform_visitor::visit(var_decl_expr_node &node) {
			return node;
		}

		var_expr_node& func_transform_visitor::visit(var_expr_node &node) {
			return node;
		}

		param_list_node& func_transform_visitor::visit(param_list_node &node) {
			return node;
		}

		call_expr_node& func_transform_visitor::visit(call_expr_node &node) {
			return node;
		}

		vector<unique_ptr<func_decl_node>> func_transform_visitor::get_decls() {
			return move(mFuncDecls);
		}
	}
}

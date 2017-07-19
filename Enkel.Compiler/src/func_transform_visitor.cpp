#include "stdafx.h"
#include "enkel_stl.h"
#include "func_transform_visitor.h"
#include "nodes.h"

using namespace std;

namespace enkel {
	namespace compiler {
		void func_transform_visitor::visit(module_node &node) {
			for (auto &elem : node.get_elems()) {
				elem->accept(*this);
			}
		}

		void func_transform_visitor::visit(module_elem_node &node) {
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
		}

		vector<unique_ptr<func_decl_node>> func_transform_visitor::get_decls() {
			return move(mFuncDecls);
		}
	}
}

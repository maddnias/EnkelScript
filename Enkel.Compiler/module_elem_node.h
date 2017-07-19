#pragma once
#include "base_node.h"
#include <vector>

namespace enkel {
	namespace compiler {
		class module_elem_node : public base_node {
		public:
			module_elem_node() {
				
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			void add_node(std::unique_ptr<base_node> &node) {
				mNodes.push_back(move(node));
			}

			std::vector<std::unique_ptr<base_node>>& get_elems() {
				return mNodes;
			}

			void remove_elem(int idx) {
				mNodes.erase(mNodes.begin()+idx);
			}

		private:
			std::vector<std::unique_ptr<base_node>> mNodes;
		};
	}
}

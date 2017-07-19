#pragma once
#include "base_node.h"
#include "param_node.h"
#include <vector>

namespace enkel {
	namespace compiler {
		class param_list_node
			: public base_node {
		public:
			param_list_node() {
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			std::vector<std::unique_ptr<param_node>>& get_params() {
				return mNodes;
			}

			void add_param(std::unique_ptr<param_node> node) {
				mNodes.push_back(move(node));
			}

		private:
			std::vector<std::unique_ptr<param_node>> mNodes;
		};
	}
}

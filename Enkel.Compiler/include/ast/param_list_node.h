#pragma once
#include "base_node.h"
#include "param_node.h"
#include <vector>
#include <assert.h>

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

			void bind_args(const std::vector<std::unique_ptr<base_node>>& args) {
				assert(args.size() == mNodes.size());
				for(int i = 0;i < args.size();i++) {
					assert(dynamic_cast<expr_node*>(args[i].get()));
					mNodes[i]->set_val(dynamic_cast<expr_node*>(args[i].get())->get_val());
				}
			}

		private:
			std::vector<std::unique_ptr<param_node>> mNodes;
		};
	}
}

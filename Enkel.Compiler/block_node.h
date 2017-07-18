#pragma once
#include "base_node.h"
#include <vector>
#include <memory>

namespace enkel {
	namespace compiler {
		class block_node : public base_node
		{
		public:
			block_node() {
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			std::string dump() override {
				return "";
			}

			void add_stmt(std::unique_ptr<base_node> stmt) {
				mStatements.push_back(move(stmt));
			}

			std::vector<std::unique_ptr<base_node>>& get_statements() {
				return mStatements;
			}

		private:
			std::vector<std::unique_ptr<base_node>> mStatements;
		//	std::unique_ptr<std::vector<std::unique_ptr<func_decl_node>>> mFuncDecls;
		};
	}
}

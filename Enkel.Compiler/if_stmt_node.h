#pragma once
#include "base_node.h"
#include <vector>
#include <memory>

namespace enkel {
	namespace compiler {
		class if_stmt_node 
			: public base_node {
		public:
			if_stmt_node(std::unique_ptr<base_node> body)
				: mBody(move(body)) {
			}

			void add_else_stmt(std::unique_ptr<base_node> stmt) {
				mElseChain.push_back(move(stmt));
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

		private:
			std::unique_ptr<base_node> mBody;
			std::vector<std::unique_ptr<base_node>> mElseChain;
		};
	}
}

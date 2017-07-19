#pragma once
#include "base_node.h"

namespace enkel {
	namespace compiler {
		class assignment_stmt_node : public base_node {

		public:
			assignment_stmt_node(std::unique_ptr<base_node> lhs, std::unique_ptr<base_node> rhs)
				: mLhs(move(lhs)),
				mRhs(move(rhs)) {
			}

			~assignment_stmt_node() override {
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

		private:
			std::unique_ptr<base_node> mLhs;
			std::unique_ptr<base_node> mRhs;
		};
	}
}

#pragma once
#include "base_node.h"

namespace enkel {
	namespace compiler {
		class assign_expr_node : public base_node {
		public:
			assign_expr_node(std::unique_ptr<base_node> lhs,
			                 std::unique_ptr<base_node> rhs)
				: mLhs(move(lhs)),
				  mRhs(move(rhs)) {
			}

			void accept(base_ast_visitor &visitor) override {}
		private:
			std::unique_ptr<base_node> mLhs;
			std::unique_ptr<base_node> mRhs;
		};
	}
}

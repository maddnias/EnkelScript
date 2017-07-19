#pragma once
#include "base_node.h"

namespace enkel {
	namespace compiler {
		class return_expr_node : public expr_node {
		public:
			return_expr_node(std::unique_ptr<base_node> retExpr)
				: mRetExpr(move(retExpr)) {
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}
			std::unique_ptr<base_node>& get_ret_expr() {
				return mRetExpr;
			}
		private:
			std::unique_ptr<base_node> mRetExpr;
		};
	}
}

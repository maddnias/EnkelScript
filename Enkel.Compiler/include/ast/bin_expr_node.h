#pragma once
#include "base_ast_visitor.h"
#include "base_node.h"
#include "expr_node.h"
#include <memory>

namespace enkel {
	namespace compiler {
		class bin_expr_node : public expr_node {
		public:
			enum bin_op {
				BIN_OP_PLUS
			};

			bin_expr_node(bin_op op, std::unique_ptr<base_node> lhs,
			              std::unique_ptr<base_node> rhs)
				:  mOp(op),
				  mLhs(move(lhs)),
				  mRhs(move(rhs)) {
			}

			std::unique_ptr<base_node>& get_lhs() {
				return mLhs;
			}

			std::unique_ptr<base_node>& get_rhs() {
				return mRhs;
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			bin_op get_op() const {
				return mOp;
			}
		private:
			bin_op mOp;
			std::unique_ptr<base_node> mLhs;
			std::unique_ptr<base_node> mRhs;

		};
	}
}

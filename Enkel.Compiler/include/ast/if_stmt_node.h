#pragma once
#include "base_node.h"
#include <vector>
#include <memory>

namespace enkel {
	namespace compiler {
		class if_stmt_node
			: public base_node {
		public:
			if_stmt_node(std::unique_ptr<base_node> cond, std::unique_ptr<base_node> trueBlock,
				std::unique_ptr<base_node> falseBlock)
				: mCond(move(cond)),
				mTrueBlock(move(trueBlock)),
				mFalseBlock(move(falseBlock)) {
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			std::unique_ptr<base_node>& get_cond() {
				return mCond;
			}

			std::unique_ptr<base_node>& get_true_block() {
				return mTrueBlock;
			}

			std::unique_ptr<base_node>& get_false_block() {
				return mFalseBlock;
			}

		private:
			std::unique_ptr<base_node> mCond;
			std::unique_ptr<base_node> mTrueBlock;
			std::unique_ptr<base_node> mFalseBlock;
		};
	}
}
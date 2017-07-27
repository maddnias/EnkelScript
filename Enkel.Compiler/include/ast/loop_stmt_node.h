#pragma once
#include "base_node.h"
#include <vector>

namespace enkel {
	namespace compiler {
		class loop_stmt_node
			: public base_node {

		public:
			loop_stmt_node(std::unique_ptr<base_node> decl,
				std::unique_ptr<base_node> incr,
				std::unique_ptr<base_node> cond,
				std::unique_ptr<base_node> body)
				: mDecl(move(decl)),
				mIncr(move(incr)),
				mCond(move(cond)),
				mBody(move(body)) {
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			std::unique_ptr<base_node> &get_decl() {
				return mDecl;
			}

			std::unique_ptr<base_node> &get_incr() {
				return mIncr;
			}

			std::unique_ptr<base_node> &get_cond() {
				return mCond;
			}

			std::unique_ptr<base_node> &get_body() {
				return mBody;
			}

		private:
			std::unique_ptr<base_node> mDecl;
			std::unique_ptr<base_node> mIncr;
			std::unique_ptr<base_node> mCond;
			std::unique_ptr<base_node> mBody;
		};
	}
}

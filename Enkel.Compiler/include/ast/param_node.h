#pragma once
#include "base_node.h"

namespace enkel {
	namespace compiler {
		class param_node : public expr_node {

		public:
			param_node(const std::wstring &ident, bool isByRef = false)
				: mByRef(isByRef),
				mIdent(ident) {
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			std::wstring &get_name() {
				return mIdent;
			}

			bool is_byref() const {
				return mByRef;
			}

		private:
			bool mByRef;
			std::wstring mIdent;
		};
	}
}

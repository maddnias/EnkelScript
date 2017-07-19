#pragma once
#include "base_node.h"

namespace enkel {
	namespace compiler {
		class param_node : public base_node {

		public:
			param_node(const std::wstring &ident)
				: mIdent(ident) {

			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

			const std::wstring& get_name() const {
				return mIdent;
			}

		private:
			std::wstring mIdent;
		};
	}
}

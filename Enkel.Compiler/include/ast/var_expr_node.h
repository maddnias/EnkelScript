#pragma once
#include "base_node.h"
#include "expr_node.h"

namespace enkel {
	namespace compiler {
		class var_expr_node : public expr_node {
		public:
			var_expr_node(const std::wstring &name)
				: mName(name) {
			}

			const std::wstring &get_name() const {
				return mName;
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}
		private:
			std::wstring mName;
		};
	}
}

#pragma once
#include "base_node.h"
#include "expr_node.h"

namespace enkel {
	namespace compiler {
		class const_expr_node : public expr_node {
		public:
			const_expr_node(runtime::variant_datatype val)
				: expr_node(val) {
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}
		};
	}
}

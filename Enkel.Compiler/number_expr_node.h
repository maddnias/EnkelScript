#pragma once
#include "base_node.h"
#include <string>
#include "expr_node.h"

namespace enkel {
	namespace compiler {
		class number_expr_node : public expr_node {
		public:
			number_expr_node(int val)
				: expr_node(runtime::variant_datatype(val)) {
			}

			std::string dump() override {
				return "";
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}

		private:
		};
	}
}

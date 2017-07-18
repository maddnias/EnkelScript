#pragma once
#include "base_node.h"
#include "expr_node.h"

namespace enkel {
	namespace compiler {
		class var_expr_node : public expr_node {
		public:
			var_expr_node(const std::string &name)
				: mName(name) {
			}

			const std::string &get_name() const {
				return mName;
			}

			std::string dump() override {
				return mName;
			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}
		private:
			std::string mName;
		};
	}
}

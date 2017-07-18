#pragma once
#include "base_node.h"

namespace enkel {
	namespace compiler {
		class param_node : public base_node {

		public:
			param_node(const std::string &ident)
				: mIdent(ident) {

			}

			void accept(base_ast_visitor &visitor) override {
				visitor.visit(*this);
			}
			
			std::string dump() override {
				return "";
			}

			const std::string& get_name() const {
				return mIdent;
			}

		private:
			std::string mIdent;
		};
	}
}

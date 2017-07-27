#pragma once
#include "base_node.h"
#include "variant_datatype.h"

namespace enkel {
	namespace compiler {
		class expr_node
			: public base_node {
		public:
			expr_node() {
			}

			expr_node(runtime::variant_datatype val)
				: mVal(std::make_unique<runtime::variant_datatype>(val)) {
			}

			void set_val(runtime::variant_datatype data) {
				mVal = std::make_unique<runtime::variant_datatype>(data);
			}

			virtual runtime::variant_datatype &get_val() {
				return *mVal;
			}

			void accept(base_ast_visitor &visitor) override = 0;
		protected:
			std::unique_ptr<runtime::variant_datatype> mVal;
		};
	}
}

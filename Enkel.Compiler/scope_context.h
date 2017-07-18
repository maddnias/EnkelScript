#pragma once
#include <vector>
#include "rt_var.h"
#include <memory>

namespace enkel {
	namespace runtime {
		class scope_context
		{
		public:
			scope_context();
			~scope_context();

			void set_var(std::string ident, variant_datatype data);
			variant_datatype& get_var(std::string ident);
			bool var_exists(const std::string & ident);

		private:
			std::shared_ptr<rt_var> find_var(std::string ident);

			std::vector<std::shared_ptr<rt_var>> mVars;
		};
	}
}

#pragma once
#include <memory>
#include "rt_var.h"
#include <vector>

namespace enkel {
	namespace runtime {
		class scope_context
		{
		public:
			scope_context();
			~scope_context();

			void set_create_var(std::wstring ident, variant_datatype data, class enkel_runtime &rt);
			std::shared_ptr<rt_var> get_var(std::wstring ident);
			variant_datatype& get_var_data(std::wstring ident);
			bool var_exists(const std::wstring &ident);
			void add_var(std::shared_ptr<rt_var> var);

		private:
			std::shared_ptr<rt_var> find_var(std::wstring ident);

			std::vector<std::shared_ptr<rt_var>> mVars;
		};
	}
}

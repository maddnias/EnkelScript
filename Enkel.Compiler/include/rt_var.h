#pragma once
#include <string>
#include "variant_datatype.h"
#include <memory>

namespace enkel {
	namespace runtime {
		class rt_var
		{
		public:
			rt_var(class enkel_runtime &rt);
			rt_var(class enkel_runtime &rt, std::wstring &name);
			~rt_var();
			
			std::wstring &get_name();
			variant_datatype &get_data();
			void set_data(const variant_datatype &data);
			bool is_ref() const;
			std::shared_ptr<rt_var> create_ref() const;
			std::shared_ptr<rt_var> create_ref(std::wstring ident) const;
			int get_scope_level() const;
			std::wstring& get_ref_name();

		private:
			variant_datatype mData;
			std::wstring mName;
			std::wstring mRefName;
			int mScopeLevel;
			bool mIsRef;
			class enkel_runtime &mRuntime;
		};
	}
}

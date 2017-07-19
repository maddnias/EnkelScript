#pragma once
#include <string>
#include "variant_datatype.h"

namespace enkel {
	namespace runtime {
		class rt_var
		{
		public:
			rt_var();
			rt_var(std::wstring &name);
			~rt_var();
			
			std::wstring &get_name();
			variant_datatype &get_data();
			void set_data(const variant_datatype& data);

		private:
			variant_datatype mData;
			std::wstring mName;
		};
	}
}

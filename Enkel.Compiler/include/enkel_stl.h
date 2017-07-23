#pragma once
#include "variant_datatype.h"
#include <vector>
#include <map>

namespace enkel {
	class enkel_stl
	{
	public:
		static bool is_stl_macro(const std::wstring &name);
		static bool is_stl_func(const std::wstring &name);
		static runtime::variant_datatype get_const(const std::wstring &name);
		static void print(std::wostream &outStream, runtime::variant_datatype &data);
		static std::wstring is_stl_const_val(runtime::variant_datatype& val);

	private:
		static std::vector<std::wstring> mStlFuncNames;
		static std::map<std::wstring, runtime::variant_datatype> mStlMacros;
	};
}

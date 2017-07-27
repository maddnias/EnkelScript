#pragma once
#include "variant_datatype.h"
#include <vector>
#include <map>
#include "ast/base_node.h"
#include <memory>
#include <vector>

namespace enkel {
	class enkel_stl
	{
	public:
		static bool is_stl_macro(const std::wstring &name);
		static bool is_stl_func(const std::wstring &name);
		static runtime::variant_datatype get_const(const std::wstring &name);
		static std::wstring is_stl_const_val(runtime::variant_datatype& val);

		static void print(std::wostream &outStream, runtime::variant_datatype &data);
		static void cast_int(runtime::variant_datatype &data);
		static void cast_str(runtime::variant_datatype &data);
		static void cast_long(runtime::variant_datatype &data);
		static void cast_double(runtime::variant_datatype &data);
		static std::wstring get_type_name(runtime::variant_datatype &data);
		static bool is_number(runtime::variant_datatype &data);

	private:
		static std::vector<std::wstring> mStlFuncNames;
		static std::map<std::wstring, runtime::variant_datatype> mStlMacros;

	};
}

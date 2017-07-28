#pragma once
#include "variant_datatype.h"
#include <vector>
#include <map>
#include <functional>
#include "../enkel_stl_param_wrapper.h"
#include <stack>

namespace enkel {
	class enkel_stl
	{
	public:
		using StlParamCont = std::stack<std::shared_ptr<runtime::enkel_stl_param_wrapper>>;
		using StlFunc = std::function<runtime::variant_datatype(StlParamCont)>;

		static bool is_stl_macro(const std::wstring &name);
		static bool is_stl_func(const std::wstring &name);
		static runtime::variant_datatype get_const(const std::wstring &name);
		static std::wstring is_stl_const_val(runtime::variant_datatype& val);
		static runtime::variant_datatype exec_func(const std::wstring &name, StlParamCont &params);

		static runtime::variant_datatype cast_int(StlParamCont params);
		static runtime::variant_datatype cast_str(StlParamCont params);
		static runtime::variant_datatype cast_long(StlParamCont params);
		static runtime::variant_datatype cast_double(StlParamCont params);
		static runtime::variant_datatype get_type_name(StlParamCont params);
		static runtime::variant_datatype is_number(StlParamCont params);

	private:
		static std::vector<std::wstring> mStlFuncNames;
		static std::map<std::wstring, runtime::variant_datatype> mStlMacros;
		static std::map<std::wstring, StlFunc> mStlFuncMap;

		static runtime::variant_datatype print(StlParamCont params);
	};
}

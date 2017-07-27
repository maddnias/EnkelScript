#include "stdafx.h"
#include "enkel_stl.h"
#include <cassert>

using namespace std;
using namespace enkel::runtime;

namespace enkel {
	vector<wstring> enkel_stl::mStlFuncNames = { 
		L"print",
		L"int",
		L"long",
		L"str",
		L"double",
		L"type",
		L"isnum"
	};

	map<wstring, variant_datatype> enkel_stl::mStlMacros = {
		make_pair<wstring, variant_datatype>(L"NewLine", variant_datatype(L"\n"))
	};

	bool enkel_stl::is_stl_macro(const wstring &name) {
		return mStlMacros.find(name) != mStlMacros.end();
	}

	bool enkel_stl::is_stl_func(const wstring &name) {
		for(auto &stlFuncName : mStlFuncNames) {
			if(stlFuncName == name) {
				return true;
			}
		}
		return false;
	}

	variant_datatype enkel_stl::get_const(const wstring &name) {
		assert(is_stl_macro(name));
		return mStlMacros[name];
	}

	void enkel_stl::print(wostream &outStream, variant_datatype &data) {
		//wstring strData = data.val_as_string();
		outStream << data;
	}

	void enkel_stl::cast_int(variant_datatype &data) {
		data.change_to_i32();
	}

	void enkel_stl::cast_str(variant_datatype &data) {
		data.change_to_str();
	}

	void enkel_stl::cast_long(variant_datatype &data) {
		data.change_to_i64();
	}

	void enkel_stl::cast_double(variant_datatype &data) {
		data.change_to_double();
	}

	wstring enkel_stl::get_type_name(variant_datatype &data) {
		switch(data.get_type()) {
		case variant_datatype::VAR_TYPE_STRING: 
			return L"string";

		case variant_datatype::VAR_TYPE_I32:
			return L"int";

		case variant_datatype::VAR_TYPE_I64: 
			return L"long";

		case variant_datatype::VAR_TYPE_UI64:
			return L"ulong";

		case variant_datatype::VAR_TYPE_DOUBLE:
			return L"double";

		default: 
			return L"unknown";
		}
	}

	bool enkel_stl::is_number(variant_datatype &data) {
		auto dataType = data.get_type();
		return dataType == variant_datatype::VAR_TYPE_I32
			|| dataType == variant_datatype::VAR_TYPE_I64
			|| dataType == variant_datatype::VAR_TYPE_DOUBLE;
	}

	wstring enkel_stl::is_stl_const_val(variant_datatype &val) {
		for(auto &constPair : mStlMacros) {
			if(constPair.second == val) {
				return constPair.first;
			}
		}
		return wstring();
	}
}

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

	map<wstring, enkel_stl::StlFunc> enkel_stl::mStlFuncMap = {
		make_pair<wstring, StlFunc>(L"print", &enkel_stl::print),
		make_pair<wstring, StlFunc>(L"int", &enkel_stl::cast_int),
		make_pair<wstring, StlFunc>(L"long", &enkel_stl::cast_long),
		make_pair<wstring, StlFunc>(L"str", &enkel_stl::cast_str),
		make_pair<wstring, StlFunc>(L"double", &enkel_stl::cast_double),
		make_pair<wstring, StlFunc>(L"type", &enkel_stl::get_type_name),
		make_pair<wstring, StlFunc>(L"isnum", &enkel_stl::is_number)
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

	variant_datatype enkel_stl::print(StlParamCont params) {
		assert(params.size() == 2);
		auto data = params.top();
		params.pop();
		params.top()->get_out_stream() << data->get_variant();

		return data->get_variant();
	}

	variant_datatype enkel_stl::cast_int(StlParamCont params) {
		assert(params.size() == 1);
		auto data = params.top();
		data->get_variant().change_to_i32();

		return data->get_variant();
	}

	variant_datatype enkel_stl::cast_str(StlParamCont params) {
		assert(params.size() == 1);
		auto data = params.top();
		data->get_variant().change_to_str();

		return data->get_variant();
	}

	variant_datatype enkel_stl::cast_long(StlParamCont params) {
		assert(params.size() == 1);
		auto data = params.top();
		data->get_variant().change_to_i64();

		return data->get_variant();
	}

	variant_datatype enkel_stl::cast_double(StlParamCont params) {
		assert(params.size() == 1);
		auto data = params.top();
		data->get_variant().change_to_double();

		return data->get_variant();
	}

	variant_datatype enkel_stl::get_type_name(StlParamCont params) {
		assert(params.size() == 1);

		auto data = params.top();
		switch(data->get_variant().get_type()) {
		case variant_datatype::VAR_TYPE_STRING: 
			return variant_datatype(L"string");

		case variant_datatype::VAR_TYPE_I32:
			return variant_datatype(L"int");

		case variant_datatype::VAR_TYPE_I64: 
			return variant_datatype(L"long");

		case variant_datatype::VAR_TYPE_UI64:
			return variant_datatype(L"ulong");

		case variant_datatype::VAR_TYPE_DOUBLE:
			return variant_datatype(L"double");

		default: 
			return variant_datatype(L"unknown");
		}
	}

	variant_datatype enkel_stl::is_number(StlParamCont params) {
		assert(params.size() == 1);
		auto data = params.top();

		return data->get_variant().is_num();
	}

	wstring enkel_stl::is_stl_const_val(variant_datatype &val) {
		for(auto &constPair : mStlMacros) {
			switch (constPair.second.get_type()) {
			case variant_datatype::VAR_TYPE_STRING:
				if(constPair.second.val_as_string() == val.val_as_string()) {
					return constPair.first;
				}
				break;
			case variant_datatype::VAR_TYPE_I32:
				if(constPair.second.val_as_i32() == val.val_as_i32()) {
					return constPair.first;
				}
				break;
			}
		}
		return wstring();
	}

	variant_datatype enkel_stl::exec_func(const wstring &name, StlParamCont &params) {
		// Assumes the name is verified to exist
		return mStlFuncMap[name](params);
	}
}

#include "stdafx.h"
#include "enkel_stl.h"
#include <cassert>

using namespace std;
using namespace enkel::runtime;

namespace enkel {
	vector<wstring> enkel_stl::mStlFuncNames = { 
		L"print"
	};

	map<wstring, variant_datatype> enkel_stl::mStlConsts = {
		make_pair<wstring, variant_datatype>(L"NewLine", variant_datatype(L"\n"))
	};

	bool enkel_stl::is_stl_const(const wstring &name) {
		return mStlConsts.find(name) != mStlConsts.end();
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
		assert(is_stl_const(name));
		return mStlConsts[name];
	}

	void enkel_stl::print(wostream &outStream, variant_datatype &data) {
		wstring strData = data.to_string();
		outStream << strData;
	}

	wstring enkel_stl::is_stl_const_val(variant_datatype &val) {
		for(auto &constPair : mStlConsts) {
			if(constPair.second == val) {
				return constPair.first;
			}
		}
		return wstring();
	}
}

#include "stdafx.h"
#include "enkel_stl.h"

using namespace std;

namespace enkel {
	vector<wstring> enkel_stl::mStlFuncNames = { 
		L"print"
	};

	bool enkel_stl::is_stl_func(wstring &name) {
		for(auto &stlFuncName : mStlFuncNames) {
			if(stlFuncName == name) {
				return true;
			}
		}
		return false;
	}

	void enkel_stl::print(wostream &outStream, runtime::variant_datatype &data) {
		outStream << data;
	}
}

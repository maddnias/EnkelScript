#include "stdafx.h"
#include "enkel_stl.h"

using namespace std;

namespace enkel {
	vector<string> enkel_stl::mStlFuncNames = { 
		"print"
	};

	bool enkel_stl::is_stl_func(string &name) {
		for(auto &stlFuncName : mStlFuncNames) {
			if(stlFuncName == name) {
				return true;
			}
		}
		return false;
	}

	void enkel_stl::print(std::ostream &outStream, runtime::variant_datatype &data) {
		outStream << data;
	}
}

#include "stdafx.h"
#include "enkel_module.h"

using namespace std;

namespace enkel {
	namespace runtime {
		enkel_module::enkel_module()
			: mFuncTbl(){
		}

		enkel_module::~enkel_module() {
		}

		bool enkel_module::func_exists(wstring &name) {
			return mFuncTbl.find(name) != mFuncTbl.end();
		}

		void enkel_module::add_func(unique_ptr<compiler::func_decl_node> func) {
			// TODO: check if exists?
			auto pFunc = move(func);
			mFuncTbl[pFunc->get_name()] = move(pFunc);
		}

		shared_ptr<compiler::func_decl_node>& enkel_module::resolve_func(wstring &name) {
			FuncTable::iterator foundFunc;
			if ((foundFunc = mFuncTbl.find(name)) != mFuncTbl.end()) {
				return foundFunc->second;
			}

			throw runtime_error("Function could not be resolved");
		}
	}
}

#include "stdafx.h"
#include <cassert>
#include "scope_context.h"
#include "enkel_runtime.h"

using namespace std;

namespace enkel {
	namespace runtime {
		scope_context::scope_context() {
		}


		scope_context::~scope_context() {
		}

		variant_datatype& scope_context::get_var_data(wstring ident) {
			//assert(find_var(ident));
			return find_var(ident)->get_data();
		}

		bool scope_context::var_exists(const wstring &ident) {
			return find_var(ident) != nullptr;
		}

		void scope_context::add_var(shared_ptr<rt_var> var) {
			mVars.push_back(var);
		}

		shared_ptr<rt_var> scope_context::find_var(wstring ident) {
			for (auto varIt : mVars) {
				if (varIt->get_name() == ident) {
					return varIt;
				}
			}
			return nullptr;
		}

		//TODO: optimize
		void scope_context::set_create_var(wstring ident, variant_datatype data, enkel_runtime &rt) {
			if(!find_var(ident)) {
				mVars.push_back(make_shared<rt_var>(rt, ident));
			}
			find_var(ident)->set_data(data);
		}

		shared_ptr<rt_var> scope_context::get_var(wstring ident) {
			return find_var(ident);
		}
	}
}

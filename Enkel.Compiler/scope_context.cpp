#include "stdafx.h"
#include "scope_context.h"
#include <cassert>

using namespace std;

namespace enkel {
	namespace runtime {
		scope_context::scope_context() {
		}


		scope_context::~scope_context() {
		}

		variant_datatype& scope_context::get_var(string ident) {
			assert(find_var(ident));
			return find_var(ident)->get_data();
		}

		bool scope_context::var_exists(const string &ident) {
			return find_var(ident) != nullptr;
		}

		shared_ptr<rt_var> scope_context::find_var(string ident) {
			for (auto varIt : mVars) {
				if (varIt->get_name() == ident) {
					return varIt;
				}
			}
			return nullptr;
		}

		//TODO: optimize
		void scope_context::set_var(string ident, variant_datatype data) {
			if(!find_var(ident)) {
				mVars.push_back(make_shared<rt_var>(ident));
			}
			find_var(ident)->set_data(data);
		}
	}
}

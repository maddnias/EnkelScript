#include "stdafx.h"
#include "rt_var.h"
#include "enkel_runtime.h"
#include <cassert>

using namespace std;

namespace enkel {
	namespace runtime {
		rt_var::rt_var(enkel_runtime &rt)
			: mIsRef(false),
			  mRuntime(rt){
			mScopeLevel = mRuntime.get_scope_level();
		}

		rt_var::rt_var(enkel_runtime &rt, wstring &name)
			: mData(),
			  mName(name),
			  mIsRef(false),
			  mRuntime(rt) {
			mScopeLevel = mRuntime.get_scope_level();
		}

		rt_var::~rt_var() {
		}

		wstring& rt_var::get_name() {
			return mName;
		}

		variant_datatype& rt_var::get_data() {
			if (mIsRef) {
				return mRuntime.get_var(mRefName)->get_data();
			}
			return mData;
		}

		void rt_var::set_data(const variant_datatype &data) {
			if (mIsRef) {
				mRuntime.set_var(mRefName, data);
			}
			else {
				mData = data;
			}
		}

		bool rt_var::is_ref() const {
			return mIsRef;
		}

		shared_ptr<rt_var> rt_var::create_ref(wstring &ident) const {
			auto ref = make_shared<rt_var>(mRuntime, ident);
			ref->mIsRef = true;
			ref->mRefName = mName;
			ref->mScopeLevel = mRuntime.get_scope_level();

			return ref;
		}

		int rt_var::get_scope_level() const {
			return mScopeLevel;
		}

		wstring& rt_var::get_ref_name() {
			return mRefName;
		}
	}
}

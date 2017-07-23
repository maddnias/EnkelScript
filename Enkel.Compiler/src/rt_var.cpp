#include "stdafx.h"
#include "rt_var.h"
#include "enkel_runtime.h"
#include <cassert>

using namespace std;

namespace enkel {
	namespace runtime {
		rt_var::rt_var(enkel_runtime &rt)
			: mIsRef(false),
			mRuntime(rt) {
		}

		rt_var::rt_var(enkel_runtime &rt, wstring &name)
			: mData(),
			mName(name),
			mIsRef(false),
			mRuntime(rt) {
		}

		rt_var::~rt_var() {
		}

		wstring& rt_var::get_name() {
			return mName;
		}

		variant_datatype& rt_var::get_data() {
			if(mIsRef) {
				return mRuntime.get_var_data(mRefName);
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

		rt_var rt_var::create_ref(wstring &ident) const {
			rt_var ref(mRuntime, ident);
			ref.mIsRef = true;
			ref.mRefName = mName;

			return ref;
		}
	}
}

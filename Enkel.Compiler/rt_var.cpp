#include "stdafx.h"
#include "rt_var.h"

namespace enkel {
	namespace runtime {
		rt_var::rt_var() {
		}

		rt_var::rt_var(std::string &name)
			: mData(),
			mName(name) {
		}

		rt_var::~rt_var() {
		}

		std::string& rt_var::get_name() {
			return mName;
		}

		variant_datatype& rt_var::get_data() {
			return mData;
		}

		void rt_var::set_data(const variant_datatype &data) {
			mData = data;
		}
	}
}

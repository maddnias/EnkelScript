#pragma once
#include <ostream>
#include "variant_datatype.h"
#include <vector>

namespace enkel {
	class enkel_stl
	{
	public:
		static bool is_stl_func(std::string &name);
		static void print(std::ostream &outStream, runtime::variant_datatype &data);

	private:
		static std::vector<std::string> mStlFuncNames;
	};
}

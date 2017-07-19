#pragma once
#include "variant_datatype.h"
#include <vector>

namespace enkel {
	class enkel_stl
	{
	public:
		static bool is_stl_func(std::wstring &name);
		static void print(std::wostream &outStream, runtime::variant_datatype &data);

	private:
		static std::vector<std::wstring> mStlFuncNames;
	};
}

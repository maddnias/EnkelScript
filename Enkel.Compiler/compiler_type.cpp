#include "stdafx.h"
#include "compiler_type.h"
#include <cassert>

using namespace std;

namespace enkel {
	namespace compiler {
		compiler_type::compiler_type()
		{
		}


		compiler_type::~compiler_type()
		{
		}

		string compiler_type::to_str(type type) {
			assert(mTypeMappings.find(type) != mTypeMappings.end());
			return mTypeMappings[type];
		}

		compiler_type::type compiler_type::from_str(const string &str) {
			//TODO: tolower?
			if (str == "string") {
				return COMPILER_TYPE_STRING;
			}
			if (str == "int" || str == "integer") {
				return COMPILER_TYPE_INT;
			}
			if (str == "nptr") {
				return COMPILER_TYPE_PTR;
			}
			throw invalid_argument("str not a valid type string");
		}
	}
}
#pragma once
#include <map>

namespace enkel {
	namespace compiler {

		static std::map<int, std::string> mTypeMappings = {
			{0, "__rtd"},
			{1, "string"},
			{2, "int"},
			{3, "ptr"}
		};

		class compiler_type {
		public:
			enum type : int {
				COMPILER_TYPE_RTD,
				COMPILER_TYPE_STRING,
				COMPILER_TYPE_INT,
				COMPILER_TYPE_PTR
			};

			compiler_type();
			~compiler_type();

			static std::string to_str(type type);
			static type from_str(const std::string &str);
		};
	}
}

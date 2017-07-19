#pragma once
#include <string>

namespace enkel {
	namespace compiler {
		class compiler_logger {
		public:
			virtual ~compiler_logger() = default;

			enum error_level {
				ERR_LVL_DEBUG,
				ERR_LVL_WARN,
				ERR_LVL_ERROR
			};

			compiler_logger() {}
		//	virtual ~compiler_logger();
			
			virtual void debug(std::wstring &data) = 0;
			virtual void warn(std::wstring &data) = 0;
			virtual void error(std::wstring &data) = 0;
		};
	}
}

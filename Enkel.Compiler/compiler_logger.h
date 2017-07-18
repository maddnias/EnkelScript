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
			
			virtual void debug(std::string &data) = 0;
			virtual void warn(std::string &data) = 0;
			virtual void error(std::string &data) = 0;
		};
	}
}

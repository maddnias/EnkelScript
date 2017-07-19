#pragma once
#include "compiler_logger.h"

namespace enkel {
	namespace compiler {
		class compiler_err_logger : public compiler_logger
		{
		public:
			compiler_err_logger();
			~compiler_err_logger();
			void debug(std::wstring &data) override;
			void warn(std::wstring &data) override;
			void error(std::wstring &data) override;
		};
	}
}

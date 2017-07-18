#pragma once
#include <memory>
#include "enkel_jit.h"
#include "../Enkel.Compiler/parser.h"

namespace enkel {
	namespace runtime {
		class enkel_rt
		{
		public:
			enkel_rt();
			~enkel_rt();

			bool initialize();
			void exec(std::unique_ptr<std::istream> data);
			void exec_file(const std::string &file);
		private:
			std::unique_ptr<enkel_jit> mJit;

			void handle_tl_expr(compiler::parser &parser);
		};
	}
}

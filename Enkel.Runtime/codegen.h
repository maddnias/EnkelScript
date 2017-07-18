#pragma once
#include "../Enkel.Compiler/func_decl_node.h"
#include "../Enkel.Compiler/func_node.h"
#include "enkel_jit.h"
#include "../Enkel.Compiler/codegen_base.h"

namespace enkel {
	namespace runtime {
		class codegen : public compiler::codegen_base
		{
		public:
			explicit codegen(enkel_jit &jit);
			~codegen();

			llvm::Function* compile_func(compiler::func_node &node) override;
			llvm::Function* compile_func_decl(compiler::func_decl_node &node) override;
			llvm::IRBuilder<> & get_builder() override;
			llvm::LLVMContext & get_context() override;
		private:
			enkel_jit &mJit;
		};
	}
}

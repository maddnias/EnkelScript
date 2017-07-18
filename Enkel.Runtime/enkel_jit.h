#pragma once
#include <memory>
#include <functional>
#include "llvm/ExecutionEngine/Orc/IndirectionUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/IRTransformLayer.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include "../Enkel.Compiler/func_node.h"

namespace enkel {
	namespace runtime {
		class codegen;

		class enkel_jit
		{
			std::unique_ptr<llvm::TargetMachine> mTargetMachine;
			llvm::orc::RTDyldObjectLinkingLayer ObjectLayer;
			llvm::orc::IRCompileLayer<decltype(ObjectLayer), llvm::orc::SimpleCompiler> CompileLayer;

			using OptimizeFunction =
				std::function<std::shared_ptr<llvm::Module>(std::shared_ptr<llvm::Module>)>;

			llvm::orc::IRTransformLayer<decltype(CompileLayer), OptimizeFunction> OptimizeLayer;

		public:
			typedef std::map<std::string, std::unique_ptr<compiler::func_decl_node>> FuncDeclMap;
			using ModuleHandle = decltype(OptimizeLayer)::ModuleHandleT;

			enkel_jit();
			~enkel_jit();

			llvm::Error initialize(std::unique_ptr<compiler::func_node> epFunc);
			llvm::TargetMachine &get_target_machine() const;
			ModuleHandle add_mod(std::shared_ptr<llvm::Module> mod);
			bool add_func(std::unique_ptr<compiler::func_node> func);
			FuncDeclMap &get_decls();

			llvm::LLVMContext& get_llvm_ctx();
			std::shared_ptr<llvm::Module> get_cur_mod();
			void set_cur_mod(std::shared_ptr<llvm::Module> mod);
			llvm::Function* get_func(const std::string& name);
			std::map<char,int>& get_bin_op_prec();
			llvm::IRBuilder<>& get_builder() const;
			codegen& get_codegen() const;

			llvm::JITSymbol find_sym(const std::string &str);

		private:
			llvm::LLVMContext mLlvmCtx;

			const llvm::DataLayout mDataLayout;
			std::unique_ptr<llvm::orc::JITCompileCallbackManager> mCompileCallbackMgr;
			std::unique_ptr<llvm::orc::IndirectStubsManager> mIndirectStubsMgr;
			std::unique_ptr<llvm::IRBuilder<>> mBuilder;
			std::shared_ptr<llvm::Module> mCurMod;
			FuncDeclMap mDeclMap;
			std::map<char, int> mBinOpPrec;
			std::shared_ptr<codegen> mCodeGen;

			std::string mangle_name(const std::string &name) const;
			std::shared_ptr<llvm::Module> optimize_mod(std::shared_ptr<llvm::Module> mod) const;
		};
	}
}

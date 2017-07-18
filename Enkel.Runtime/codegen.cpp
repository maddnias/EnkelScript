#include "stdafx.h"
#include "codegen.h"

using namespace llvm;

namespace enkel {
	namespace runtime {
		codegen::codegen(enkel_jit &jit) 
		: codegen_base(),
			mJit(jit) {
		}

		codegen::~codegen()
		{
		}

		Function* codegen::compile_func(compiler::func_node &node) {
			auto &decl = node.get_decl();
			// Attempt to retrieve function from module
			auto *func = mJit.get_func(decl.get_name());
			if(!func) {
				return nullptr;
			}

			if(decl.isBinaryOp()) {
				mJit.get_bin_op_prec()[decl.get_op_name()] = decl.get_binary_prec();
			}

			BasicBlock *bb = BasicBlock::Create(mJit.get_llvm_ctx(), "entry", func);
			mJit.get_builder().SetInsertPoint(bb);

			for(auto &arg : func->args()) {
			
			}

			if (Value *retVal = node.get_body().codegen(*this)) {
				mJit.get_builder().CreateRet(retVal);

				verifyFunction(*func);
			}

			return func;
		}

		Function* codegen::compile_func_decl(compiler::func_decl_node &node) {
			std::vector<Type*> args(node.get_args().size(), Type::getInt32Ty(mJit.get_llvm_ctx()));
			FunctionType *funcType = FunctionType::get(Type::getVoidTy(mJit.get_llvm_ctx()), args, false);
			Function *func = Function::Create(funcType, Function::ExternalLinkage, node.get_name(), mJit.get_cur_mod().get());

			if (node.get_args().size() > 0) {
				unsigned idx = 0;
				for (auto &arg : func->args()) {
					arg.setName("test" + idx++);
				}
			}

			return func;
		}

		IRBuilder<>& codegen::get_builder() {
			return mJit.get_builder();
		}

		LLVMContext & codegen::get_context() {
			return mJit.get_llvm_ctx();
		}
	}
}
#include "stdafx.h"
#include "enkel_jit.h"
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include "llvm/ADT/STLExtras.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/RTDyldMemoryManager.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/IndirectionUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/IRTransformLayer.h"
#include "llvm/ExecutionEngine/Orc/LambdaResolver.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Mangler.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "../Enkel.Compiler/func_node.h"
#include "codegen.h"
#include <iostream>

using namespace llvm;
using namespace std;

namespace enkel {
	namespace runtime {
		enkel_jit::enkel_jit()
			: ObjectLayer([]() {
				  return make_shared<SectionMemoryManager>();
			  }),
			  CompileLayer(ObjectLayer, orc::SimpleCompiler(*mTargetMachine)),
			  OptimizeLayer(CompileLayer,
			                [this](shared_ptr<Module> M) {
			                return optimize_mod(move(M));
		                }),
			  mTargetMachine(EngineBuilder().selectTarget()),
			  mDataLayout(mTargetMachine->createDataLayout()),
			  mCompileCallbackMgr(
				  orc::createLocalCompileCallbackManager(mTargetMachine->getTargetTriple(), 0)),
			  mBuilder(std::make_unique<IRBuilder<>>(mLlvmCtx)) {

			auto IndirectStubsMgrBuilder =
				orc::createLocalIndirectStubsManagerBuilder(mTargetMachine->getTargetTriple());

			mCodeGen = make_shared<codegen>(*this);
			mIndirectStubsMgr = IndirectStubsMgrBuilder();
			sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
			mCurMod = make_shared<Module>("enkel_jit", mLlvmCtx);
			mCurMod->setDataLayout(mTargetMachine->createDataLayout());
		}

		enkel_jit::~enkel_jit() {
		}

		TargetMachine& enkel_jit::get_target_machine() const {
			return *mTargetMachine;
		}

		enkel_jit::ModuleHandle enkel_jit::add_mod(shared_ptr<Module> mod) {
			auto resolver = orc::createLambdaResolver(
				[&](const string &name) {
				if (auto sym = mIndirectStubsMgr->findStub(name, false)) {
					return sym;
				}
				if (auto sym = OptimizeLayer.findSymbol(name, false)) {
					return sym;
				}
				return JITSymbol(nullptr);
			},
				[](const string &name) {
				if (auto symAddr = RTDyldMemoryManager::getSymbolAddressInProcess(name)) {
					return JITSymbol(symAddr, JITSymbolFlags::Exported);
				}
				return JITSymbol(nullptr);
			});

			return cantFail(CompileLayer.addModule(mod, move(resolver)));
		}

		bool enkel_jit::add_func(unique_ptr<compiler::func_node> func) {
			auto ccInf = mCompileCallbackMgr->getCompileCallback();
	 for(auto &l : mCurMod->getFunctionList()) {
		 cout << l.getName().str();
	 }
			if (auto err = mIndirectStubsMgr->createStub(mangle_name("enkel_func_ep"),
			                                             ccInf.getAddress(),
			                                             JITSymbolFlags::Exported)) {
				//TODO: err
				return false;
			}

			auto sharedFuncNode = shared_ptr<compiler::func_node>(move(func));

			ccInf.setCompileAction(
				[this, sharedFuncNode]() {
				shared_ptr<Module> mod;
				if (auto *cmpldFunc = mCodeGen->compile_func(*sharedFuncNode)) {
					cmpldFunc->setName(cmpldFunc->getName() + "$impl");
					//mod = move(mCurMod);
				//	mCurMod = std::make_unique<Module>("lul", mLlvmCtx);
				}
				else {
					//TODO: err
				}

				//TODO: fix
				add_mod(nullptr);

				auto sym = OptimizeLayer.findSymbol(sharedFuncNode->get_name() + "$impl", false);
				assert(sym);
				JITTargetAddress symAddr = cantFail(sym.getAddress());
				if (auto err = mIndirectStubsMgr->updatePointer(mangle_name(sharedFuncNode->get_name()), symAddr)) {
					//TODO: err
				}
				return symAddr;
			}
			);


		}

		enkel_jit::FuncDeclMap& enkel_jit::get_decls() {
			return mDeclMap;
		}

		LLVMContext& enkel_jit::get_llvm_ctx() {
			return mLlvmCtx;
		}

		shared_ptr<Module> enkel_jit::get_cur_mod() {
			return mCurMod;
		}

		void enkel_jit::set_cur_mod(std::shared_ptr<llvm::Module> mod) {
			mCurMod = mod;
		}

	/*	void enkel_jit::set_cur_mod(unique_ptr<Module> mod) {
			mCurMod = move(mod);
		}
*/
		Function* enkel_jit::get_func(const string &name) {
			// Check if the function already exists in current module
			if (auto *func = mCurMod->getFunction(name)) {
				return func;
			}

			// If not, codegen it if possible
			auto foundDecl = mDeclMap.find(name);
			if (foundDecl != mDeclMap.end()) {
				return mCodeGen->compile_func_decl(*foundDecl->second);
			}

			// Not found
			return nullptr;
		}

		map<char, int>& enkel_jit::get_bin_op_prec() {
			return mBinOpPrec;
		}

		IRBuilder<>& enkel_jit::get_builder() const {
			return *mBuilder;
		}

		string enkel_jit::mangle_name(const string &name) const {
			string mangledName;
			raw_string_ostream MangledNameStream(mangledName);
			Mangler::getNameWithPrefix(MangledNameStream, name, mDataLayout);

			return MangledNameStream.str();
		}

		shared_ptr<Module> enkel_jit::optimize_mod(shared_ptr<Module> mod) const {
			auto fpm = llvm::make_unique<FunctionPassManager>(mod.get());

			/*	fpm->addPass(createInstructionCombiningPass());
				fpm->addPass(createReassociatePass());
				fpm->addPass(createGVNPass());
				fpm->addPass(createCFGSimplificationPass());*/

			for (auto &func : *mod) {
				//TODO: optimize
			}

			return mod;
		}

		Error enkel_jit::initialize(unique_ptr<compiler::func_node> epFunc) {
			auto ccInf = mCompileCallbackMgr->getCompileCallback();

			if (auto err = mIndirectStubsMgr->createStub(mangle_name("enkel_func_ep"),
			                                             ccInf.getAddress(),
			                                             JITSymbolFlags::Exported)) {
				return err;
			}


			/*	ccInf.setCompileAction(
					[this, ]*/
			//)
		}

		codegen& enkel_jit::get_codegen() const {
			return *mCodeGen;
		}

		JITSymbol enkel_jit::find_sym(const string &str) {
			return OptimizeLayer.findSymbol(mangle_name(str), false);
		}
	}
}

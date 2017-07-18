#include "stdafx.h"
#include "enkel_rt.h"
#include <llvm/Support/TargetSelect.h>
#include "../Enkel.Compiler/parser.h"
#include "codegen.h"

using namespace llvm;
using namespace std;

namespace enkel {
	namespace runtime {
		enkel_rt::enkel_rt() {
		}

		enkel_rt::~enkel_rt() {
		}

		bool enkel_rt::initialize() {
			InitializeNativeTarget();
			InitializeNativeTargetAsmPrinter();
			InitializeNativeTargetAsmParser();

			mJit = std::make_unique<enkel_jit>();

			return true;
		}

		void enkel_rt::exec(unique_ptr<istream> data) {
			auto lex = std::make_unique<compiler::lexer>();
			lex->initialize(data);

			// Give ownership of lexer to parser
			compiler::parser srcParser(move(lex));

			// Only gather functions first pass
			while (true) {
				auto tok = srcParser.next_token();
				unique_ptr<compiler::func_node> func;
				switch (tok->get_type()) {

				case compiler::TOK_EOF:
					break;
				case compiler::TOK_FUNC_DEF:
					continue;
					mJit->add_func(move(srcParser.parse_func()));
					break;
				default:
					handle_tl_expr(srcParser);
					break;
				}
			}
		}

		void enkel_rt::handle_tl_expr(compiler::parser &parser) {
			if (auto expr = parser.parse_expr()) {
				// Create anonymous func decl
				auto decl = std::make_unique<compiler::func_decl_node>("__anon_expr", map<string, string>());
				if (auto func = std::make_unique<compiler::func_node>(move(decl), move(expr))) {
					mJit->get_decls()[func->get_name()] = std::make_unique<compiler::func_decl_node>(func->get_decl());
					// Compile it
			//		if(mJit->get_codegen().compile_func(*func)) {
						mJit->add_func(move(func));
						//auto h = mJit->add_mod(mJit->get_cur_mod());
						//auto newMod = std::make_unique<Module>("lul", mJit->get_llvm_ctx());
						//newMod->setDataLayout(mJit->get_target_machine().createDataLayout());
						//mJit->set_cur_mod(move(newMod));

						auto exprSym = mJit->find_sym("__anon_expr");
				//	}
				}
			}
		}
	}
}

#include "stdafx.h"
#include "enkel_runtime.h"
#include "exec_visitor.h"
#include "func_transform_visitor.h"
#include <iostream>
#include "print_visitor.h"

using namespace std;

namespace enkel {
	namespace runtime {
		enkel_runtime::enkel_runtime()
			: mOutStream(wcout) /* use wcout as default */ {
		}

		enkel_runtime::enkel_runtime(wostream &outStream)
			: mOutStream(outStream) {
		}

		enkel_runtime::~enkel_runtime() {
		}

		void enkel_runtime::exec(unique_ptr<wistream> data) {
			auto lex = make_unique<compiler::lexer>();
			if (!lex->initialize(data)) {
				throw runtime_error("Failed to initialize enkel lexer");
			}

			// Give ownership of lexer to parser
			compiler::parser srcParser(move(lex));

			// Create a runtime module for this input
			auto mod = make_shared<enkel_module>();

			auto root = srcParser.parse_module();
			wcout << L"Module AST:" << endl << endl;

			compiler::print_visitor printer(wcout);
			root->accept(printer);

			compiler::func_transform_visitor extractor;
			root->accept(extractor);
			auto decls = extractor.get_decls();

			for (auto &decl : decls) {
				mod->add_func(move(decl));
			}

			/* 
			 * compiler::semantic_analyser analyser();
			 * root->accept(analyser);
			 */

			mLoadedModules.push_back(mod);

			wcout << endl << endl << L"Executing script:" << endl << endl;

			exec_visitor exec(*this);
			root->accept(exec);
		}


		void enkel_runtime::exec_file(const wstring &file) {
		}

		void enkel_runtime::scope_increase() {
			mScopes.push_back(scope_context());
		}

		void enkel_runtime::scope_decrease() {
			mScopes.pop_back();
		}

		scope_context& enkel_runtime::get_current_scope() {
			return mScopes.back();
		}

		//TODO: Assumes var refs are always in a higher scope
		shared_ptr<rt_var> enkel_runtime::get_var(wstring ident) {
			//TODO: global refs?
			if (mGlobalScope.var_exists(ident)) {
				return mGlobalScope.get_var(ident);
			}
			shared_ptr<rt_var> varRef;
			for (auto it = mScopes.rbegin(); it != mScopes.rend(); ++it) {
				if (it->var_exists(ident)) {
					if (!varRef && !(varRef = it->get_var(ident))->is_ref()) {
						// No references to it
						return varRef;
					}

					// We have a ref, attempt to find what it's pointing to
					for (int i = 0; i < varRef->get_scope_level(); i++) {
						if (mScopes.at(i).var_exists(varRef->get_ref_name())) {
							return mScopes.at(i).get_var(varRef->get_ref_name());
						}
					}
				}
			}
			//TODO: real error
			throw runtime_error("not found");
		}

		variant_datatype& enkel_runtime::get_var_data(wstring ident) {
			return get_var(ident)->get_data();
		}

		void enkel_runtime::set_var(rt_var &var) {
			set_var(var.get_name(), var.get_data());
		}

		//TODO: Assumes var refs are always in a higher scope
		void enkel_runtime::set_var(wstring ident, const variant_datatype &data) {
			if (mGlobalScope.var_exists(ident)) {
				return mGlobalScope.set_create_var(ident, data, *this);
			}
			shared_ptr<rt_var> varRef;
			for (auto it = mScopes.rbegin(); it != mScopes.rend(); ++it) {
				if (it->var_exists(ident)) {
					if (!varRef && !(varRef = it->get_var(ident))->is_ref()) {
						// No references to it
						it->set_create_var(ident, data, *this);
					}
					else {
						// We have a ref, attempt to find what it's pointing to
						for (int i = 0; i < varRef->get_scope_level(); i++) {
							if (mScopes.at(i).var_exists(varRef->get_ref_name())) {
								return mScopes.at(i).set_create_var(varRef->get_ref_name(), data, *this);
							}
						}
					}
				}
			}
			get_current_scope().set_create_var(ident, data, *this);
		}

		wostream& enkel_runtime::get_ostream() const {
			return mOutStream;
		}

		compiler::func_decl_node& enkel_runtime::resolve_func(wstring &name) {
			for (auto &mod : mLoadedModules) {
				if (mod->func_exists(name)) {
					return *mod->resolve_func(name);
				}
			}
			throw runtime_error("Failed to resolve function");
		}

		int enkel_runtime::get_scope_level() const {
			return mScopes.size();
		}

		unique_ptr<compiler::base_node> enkel_runtime::parse_tl_expr(compiler::parser &parser) {
			return nullptr;
		}

		shared_ptr<enkel_module> enkel_runtime::create_module(compiler::parser &parser) const {
			// Create a module for this input
			auto newMod = make_shared<enkel_module>();
			bool eofReached = false;

			// Gather all declared functions and add them to module's function table
			while (!eofReached) {
				auto tok = parser.next_token();
				unique_ptr<compiler::func_node> func;
				wstring funcName;

				switch (tok->get_type()) {

				case compiler::TOK_EOF:
					eofReached = true;
					break;
				case compiler::TOK_FUNC_DECL:
					//		func = move(parser.parse_func());
					funcName = func->get_name();

					break;
				default:
					break;
				}
			}

			return newMod;
		}
	}
}

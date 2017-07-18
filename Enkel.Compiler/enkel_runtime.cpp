#include "stdafx.h"
#include "enkel_runtime.h"
#include "bin_expr_node.h"
#include "module_node.h"
#include "print_visitor.h"
#include <iostream>
#include "func_transform_visitor.h"
#include "exec_visitor.h"

using namespace std;

namespace enkel {
	namespace runtime {
		enkel_runtime::enkel_runtime()
			: mOutStream(cout) /* use cout as default */ {
		}

		enkel_runtime::enkel_runtime(ostream &outStream)
			: mOutStream(outStream) {
		}

		enkel_runtime::~enkel_runtime() {
		}

		void enkel_runtime::exec(unique_ptr<istream> data) {
			auto lex = make_unique<compiler::lexer>();
			if (!lex->initialize(data)) {
				throw runtime_error("Failed to initialize enkel lexer");
			}

			// Give ownership of lexer to parser
			compiler::parser srcParser(move(lex));

			// Create a runtime module for this input
			auto mod = make_shared<enkel_module>();

			auto root = srcParser.parse_module();
			cout << "Module AST:" << endl << endl;
			compiler::print_visitor printer(cout);
			root->accept(printer);
			compiler::func_transform_visitor extractor;
			root->accept(extractor);
			auto decls = extractor.get_decls();

			for(auto &decl : decls) {
				mod->add_func(move(decl));
			}

			mLoadedModules.push_back(mod);

			cout << endl << endl << "Executing script:" << endl << endl;

			exec_visitor exec(*this);
			root->accept(exec);
		}


		void enkel_runtime::exec_file(const string &file) {
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

		variant_datatype& enkel_runtime::get_var(string ident) {
			if (mGlobalScope.var_exists(ident)) {
				return mGlobalScope.get_var(ident);
			}
			return get_current_scope().get_var(ident);
		}

		void enkel_runtime::set_var(string ident, variant_datatype &data) {
			if (mGlobalScope.var_exists(ident)) {
				return mGlobalScope.set_var(ident, data);
			}
			return get_current_scope().set_var(ident, data);
		}

		ostream& enkel_runtime::get_ostream() const {
			return mOutStream;
		}

		unique_ptr<compiler::func_decl_node>& enkel_runtime::resolve_func(string &name) {
			for(auto &mod : mLoadedModules) {
				if(mod->func_exists(name)) {
					return mod->resolve_func(name);
				}
			}
			throw runtime_error("Failed to resolve function");
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
				string funcName;

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
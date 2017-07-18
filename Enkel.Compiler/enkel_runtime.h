#pragma once
#include <memory>
#include <string>
#include "parser.h"
#include "enkel_module.h"
#include "scope_context.h"

namespace enkel {
	namespace runtime {
		class enkel_runtime
		{
		public:
			enkel_runtime();
			enkel_runtime(std::ostream &outStream);
			~enkel_runtime();

			void exec(std::unique_ptr<std::istream> data);
			void exec_file(const std::string &file);

			void scope_increase();
			void scope_decrease();
			scope_context& get_current_scope();
			variant_datatype& get_var(std::string ident);
			void set_var(std::string ident, variant_datatype &data);
			std::ostream &get_ostream() const;
			std::unique_ptr<compiler::func_decl_node>& resolve_func(std::string &name);

		private:
			std::vector<std::shared_ptr<enkel_module>> mLoadedModules;
			std::unique_ptr<compiler::parser> mParser;
			std::vector<scope_context> mScopes;
			scope_context mGlobalScope;
			std::ostream& mOutStream;

			std::unique_ptr<compiler::base_node> parse_tl_expr(compiler::parser & parser);
			//void exec_module(std::shared_ptr<enkel_module> mod, compiler::parser &parser);
			std::shared_ptr<enkel_module> create_module(compiler::parser &parser) const;
			//void handle_tl_expr(compiler::parser &parser);
			//void handle_ident_expr(compiler::parser &parser);
		};
	}
}

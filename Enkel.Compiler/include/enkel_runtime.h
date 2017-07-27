#pragma once
#include "enkel_module.h"
#include <memory>
#include "parser.h"
#include "scope_context.h"


namespace enkel {
	namespace runtime {
		class enkel_runtime
		{
		public:
			enkel_runtime();
			enkel_runtime(std::wostream &outStream);
			~enkel_runtime();

			void exec(std::unique_ptr<std::wistream> data);
			void exec_file(const std::wstring &file);

			void scope_increase();
			void scope_decrease();
			scope_context& get_current_scope();
			std::shared_ptr<rt_var> get_var(std::wstring ident);
			variant_datatype& get_var_data(std::wstring ident);
			std::shared_ptr<rt_var> set_var(rt_var &var);
			std::shared_ptr<rt_var> set_var(std::wstring ident, const variant_datatype &data);
			std::wostream &get_ostream() const;
			compiler::func_decl_node& resolve_func(std::wstring &name);
			int get_scope_level() const;

		private:
			std::vector<std::shared_ptr<enkel_module>> mLoadedModules;
			std::unique_ptr<compiler::parser> mParser;
			std::vector<scope_context> mScopes;
			scope_context mGlobalScope;
			std::wostream& mOutStream;

			std::unique_ptr<compiler::base_node> parse_tl_expr(compiler::parser & parser);
			//void exec_module(std::shared_ptr<enkel_module> mod, compiler::parser &parser);
			std::shared_ptr<enkel_module> create_module(compiler::parser &parser) const;
			//void handle_tl_expr(compiler::parser &parser);
			//void handle_ident_expr(compiler::parser &parser);
		};
	}
}

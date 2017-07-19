#pragma once
#include "lexer.h"
#include "compiler_logger.h"
#include "nodes.h"
#include <map>

namespace enkel {
	namespace compiler {
		class parser
		{
		public:
			parser(std::unique_ptr<lexer> lexer, 
				std::unique_ptr<compiler_logger> compileLogger = nullptr);
			~parser();

			std::shared_ptr<lexer_token> next_token(int count = 1);
			std::shared_ptr<lexer_token> peek_next_token(int count = 1) const;
			std::unique_ptr<module_node> parse_module();
			void reset();
			std::shared_ptr<lexer_token> get_cur_token() const;
			std::unique_ptr<base_node> parse_expr();


		private:
			using VarDeclVec = std::unique_ptr<std::vector<std::unique_ptr<var_decl_expr_node>>>;
			using FuncDeclVec = std::unique_ptr<std::vector<std::unique_ptr<func_decl_node>>>;
			using ParamsVec = std::unique_ptr<std::vector<std::unique_ptr<param_node>>>;
			using error_level = compiler_logger::error_level;

			std::unique_ptr<param_list_node> parse_params();
			std::unique_ptr<base_node> parse_unary();
			std::unique_ptr<base_node> parse_identifer();
			std::unique_ptr<base_node> parse_func_decl();
			std::unique_ptr<base_node> parse_keyword();
			std::unique_ptr<module_elem_node> parse_mod_elem();
			std::unique_ptr<base_node> parse_stmt();
			std::unique_ptr<base_node> parse_bin_op_rhs(int exprPrec, std::unique_ptr<base_node> lhs);
			std::unique_ptr<block_node> parse_block();
			std::unique_ptr<base_node> parse_var_decl();
			std::unique_ptr<base_node> parse_number();
			std::unique_ptr<base_node> parse_literal();
			std::unique_ptr<base_node> parse_primary();
			std::unique_ptr<base_node> parse_call_expr();
			std::vector<std::unique_ptr<base_node>> parse_arg_list();

			int get_tok_prec();
			void expect(const std::wstring &expected, std::wstring &actual, error_level level) const;
			void expect(lexer_tok_type expected, lexer_tok_type actual, error_level level) const;

			std::unique_ptr<lexer> mLexer;
			std::shared_ptr<lexer_token> mCurTok;
			std::map<char, int> mBinOpPrec = {
				{'=', 2},
				{'<', 10},
				{'+', 20},
				{'-', 20},
				{'*', 40}
			};
			std::unique_ptr<compiler_logger> mCompileLogger;
		};
	}
}

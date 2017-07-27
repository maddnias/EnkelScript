#pragma once
#include "compiler_logger.h"
#include "lexer.h"
#include <map>
#include "nodes.h"
#include <type_traits>

namespace enkel {
	namespace compiler {
		class parser {
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
			using BinOpPrecMap = std::map<std::wstring, int>;

			std::unique_ptr<param_list_node> parse_params();
			std::unique_ptr<base_node> parse_unary();
			std::unique_ptr<base_node> parse_identifer();
			std::unique_ptr<base_node> parse_func_decl();
			void parse_else_chain(std::unique_ptr<if_stmt_node> &node);
			std::unique_ptr<base_node> parse_if_stmt();
			std::unique_ptr<base_node> parse_loop_stmt();
			std::unique_ptr<base_node> parse_keyword();
			std::unique_ptr<module_elem_node> parse_mod_elem();
			std::unique_ptr<base_node> parse_stmt();
			std::unique_ptr<base_node> parse_bin_op_rhs(int exprPrec, std::unique_ptr<base_node> lhs);
			std::unique_ptr<block_node> parse_block();
			std::unique_ptr<base_node> parse_var_decl();
			std::unique_ptr<base_node> parse_number();
			std::unique_ptr<base_node> parse_literal();
			std::unique_ptr<base_node> parse_stl_const();
			std::unique_ptr<base_node> parse_primary();
			std::unique_ptr<base_node> parse_call_expr();
			std::vector<std::unique_ptr<base_node>> parse_arg_list();

			int get_tok_prec();
			void expect(const std::wstring &expected, const std::wstring &actual, error_level level) const;
			void expect(lexer_tok_type expected, lexer_tok_type actual, error_level level) const;

			std::unique_ptr<lexer> mLexer;
			std::shared_ptr<lexer_token> mCurTok;
			BinOpPrecMap mBinOpPrec = {
				{L"=", 2},
				{L"<", 10},
				{L">", 10},
				{L"<=", 10},
				{L">=", 10},
				{L"+", 20},
				//{'-', 20},
				{L"*", 40},
			};
			std::unique_ptr<compiler_logger> mCompileLogger;
		};
	}
}

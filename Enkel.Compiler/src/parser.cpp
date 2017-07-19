#include "stdafx.h"
#include "lexer.h"
#include "nodes.h"
#include "parser.h"

using namespace std;

namespace enkel {
	namespace compiler {
		parser::parser(unique_ptr<lexer> lexer, unique_ptr<compiler_logger> compileLogger)
			: mLexer(move(lexer)),
			mCompileLogger(move(compileLogger)) {
			next_token();
		}

		parser::~parser()
		{
		}

		shared_ptr<lexer_token> parser::next_token(int count) {
			assert(count > 0);
			for (int i = 0; i < count; i++) {
				mCurTok = mLexer->next_token();
			}
			return mCurTok;
		}

		shared_ptr<lexer_token> parser::peek_next_token(int count) const {
			assert(count > 0);
			return mLexer->peek_next_token(count);
		}

		unique_ptr<module_node> parser::parse_module() {
			auto mod = make_unique<module_node>(L"__enkel_mod_0");

			while(mCurTok->get_type() != TOK_EOF) {
				mod->add_elem(parse_mod_elem());
			}
		
			return move(mod);
		}

		void parser::reset() {
			mLexer->reset();
			next_token();
		}

		shared_ptr<lexer_token> parser::get_cur_token() const {
			return mCurTok;
		}

		unique_ptr<base_node> parser::parse_expr() {
			auto lhs = parse_unary();
			if(!lhs) {
				// Prevent infinite loop
				next_token();
				return nullptr;
			}

			if(dynamic_cast<return_expr_node*>(lhs.get())) {
				return move(lhs);
			}

			return parse_bin_op_rhs(0, move(lhs));
		}

		///* block : (variable_declaration)*
		//			| (func_declaration)*
		//			| compound_statement
		//*/
		unique_ptr<block_node> parser::parse_block() {
			auto block = make_unique<block_node>();
			bool blockEnd = false;
			while (!blockEnd) {
				switch (mCurTok->get_type()) {
				case TOK_IDENTIFIER:
					block->add_stmt(parse_identifer());
					break;
				case TOK_KEYWORD:
					block->add_stmt(parse_keyword());
					break;

				default:
					blockEnd = true;
					break;
				}
			}

			return move(block);
		}

		unique_ptr<base_node> parser::parse_number() {
			auto numExpr = make_unique<const_expr_node>(mCurTok->get_lexeme_num());
			// Eat number
			next_token();
			return move(numExpr);
		}

		unique_ptr<base_node> parser::parse_literal() {
			auto literal = make_unique<const_expr_node>(runtime::variant_datatype(mCurTok->get_lexeme_str()));
			// Eat literal
			next_token();
			return move(literal);
		}

		unique_ptr<base_node> parser::parse_primary() {
			switch (mCurTok->get_type()) {
			case TOK_IDENTIFIER:
				return parse_identifer();
			case TOK_VAR_DECL:
				//		return parse_var_decl();
			case TOK_NUMBER:
				return parse_number();
			case TOK_LITERAL:
				return parse_literal();
			case TOK_KEYWORD:
				return parse_keyword();
			}
			return nullptr;
		}

		unique_ptr<base_node> parser::parse_call_expr() {
			expect(TOK_IDENTIFIER, mCurTok->get_type(), error_level::ERR_LVL_ERROR);
			wstring ident = mCurTok->get_lexeme_str();
			// Eat ident
			next_token();
			auto callExpr = make_unique<call_expr_node>(ident);
			auto args = parse_arg_list();

			for (auto &arg : args) {
				callExpr->add_arg(move(arg));
			}

			return move(callExpr);
		}

		vector<unique_ptr<base_node>> parser::parse_arg_list() {
			expect(TOK_OPEN_PARENTH, mCurTok->get_type(), error_level::ERR_LVL_ERROR);
			// Eat '('
			next_token();

			vector<unique_ptr<base_node>> args;
			while(mCurTok->get_type() != TOK_CLOSE_PARENTH) {
				if(mCurTok->get_type() == TOK_EOF) {
					//TODO: something seriously wrong
				}
				args.push_back(parse_expr());
			}

			expect(TOK_CLOSE_PARENTH, mCurTok->get_type(), error_level::ERR_LVL_ERROR);
			// Eat ')'
			next_token();

			return args;
		}

		//	return nullptr;
		//}

		unique_ptr<base_node> parser::parse_var_decl() {
			//TODO: expect scope decl
			wstring scopeDecl;
			if(mCurTok->get_type() == TOK_VAR_DECL_SCOPE) {
				scopeDecl = mCurTok->get_lexeme_str();
				// Eat scope decl
				next_token();
			}
			expect(TOK_IDENTIFIER, mCurTok->get_type(), error_level::ERR_LVL_ERROR);
			wstring varIdent = mCurTok->get_lexeme_str();
			// Eat ident
			next_token();

			unique_ptr<base_node> initAssign = nullptr;
			if (mCurTok->get_type() == TOK_ASSIGN) {
				// Eat '='
				next_token();
				initAssign = parse_expr();
			}

			return make_unique<var_decl_expr_node>(scopeDecl, varIdent, move(initAssign));
		}


		unique_ptr<param_list_node> parser::parse_params() {
			expect(TOK_OPEN_PARENTH, mCurTok->get_type(), error_level::ERR_LVL_ERROR);
			// Eat '('
			next_token();
			auto params = make_unique<param_list_node>();
			if(mCurTok->get_type() == TOK_CLOSE_PARENTH) {
				// No params
				return move(params);
			}
			expect(TOK_IDENTIFIER, mCurTok->get_type(), error_level::ERR_LVL_ERROR);

			while(mCurTok->get_type() == TOK_IDENTIFIER
				|| mCurTok->get_type() == TOK_COMMA) {
				if(mCurTok->get_type() == TOK_IDENTIFIER) {
					params->add_param(make_unique<param_node>(mCurTok->get_lexeme_str()));
				}
				// Eat ident/comma
				next_token();
			}

			expect(TOK_CLOSE_PARENTH, mCurTok->get_type(), error_level::ERR_LVL_ERROR);
			// Eat ')'
			next_token();

			return move(params);
		}

		unique_ptr<base_node> parser::parse_unary() {
			if(mCurTok->get_type() != TOK_OP_UNARY) {
				return parse_primary();
			}

			return nullptr;
		}

		unique_ptr<base_node> parser::parse_identifer() {
			wstring identName = mCurTok->get_lexeme_str();
			
			// Check if it's a call
			if(peek_next_token()->get_type() == TOK_OPEN_PARENTH) {
				return parse_call_expr();
			}

			// Check if it's a declaration
			if(peek_next_token()->get_type() == TOK_ASSIGN) {
				return parse_var_decl();
			}

			// Eat ident
			next_token();
			return make_unique<var_expr_node>(identName);
		}

		unique_ptr<base_node> parser::parse_func_decl() {
			expect(TOK_FUNC_DECL, mCurTok->get_type(), error_level::ERR_LVL_ERROR);
			next_token();
			expect(TOK_IDENTIFIER, mCurTok->get_type(), error_level::ERR_LVL_ERROR);

			wstring funcName = mCurTok->get_lexeme_str();
			// Eat ident
			next_token();
			auto params = parse_params();
			auto body = parse_block();
			expect(TOK_END, mCurTok->get_type(), error_level::ERR_LVL_ERROR);
			// Eat 'end'
			next_token();
			return make_unique<func_decl_node>(funcName, move(params), move(body));
		}

		unique_ptr<base_node> parser::parse_keyword() {
			assert(mCurTok->get_type() == TOK_KEYWORD);
			if(mCurTok->get_lexeme_str() == L"return") {
				// Eat 'return'
				next_token();
				return make_unique<return_expr_node>(move(parse_expr()));
			}

			return nullptr;
		}

		unique_ptr<module_elem_node> parser::parse_mod_elem() {
			auto modElem = make_unique<module_elem_node>();
			while (mCurTok->get_type() != TOK_EOF) {
				if(mCurTok->get_type() == TOK_FUNC_DECL) {
					modElem->add_node(parse_func_decl());
				} else {
					modElem->add_node(parse_stmt());
				}
			}

			return move(modElem);
		}

		unique_ptr<base_node> parser::parse_stmt() {
			if(mCurTok->get_type() == TOK_VAR_DECL_SCOPE) {
				return parse_var_decl();
			}
			if(mCurTok->get_type() == TOK_IDENTIFIER) {
				return parse_identifer();
			}

			return nullptr;
		}

		unique_ptr<base_node> parser::parse_bin_op_rhs(int exprPrec, 
			unique_ptr<base_node> lhs) {
			while (true) {
				int tokPrec = get_tok_prec();

				if (tokPrec < exprPrec) {
					return lhs;
				}

				bin_expr_node::bin_op op;
				switch(mCurTok->get_lexeme_str()[0]) {
				case '+':
					op = bin_expr_node::BIN_OP_PLUS;
					break;
				default:
					//TODO: add rest
					break;
				}
				// Eat op
				next_token();

				auto rhs = parse_unary();
				if (!rhs) {
					return nullptr;
				}

				int nextPrec = get_tok_prec();
				if (tokPrec < nextPrec) {
					rhs = parse_bin_op_rhs(tokPrec + 1, move(rhs));
					if (!rhs) {
						return nullptr;
					}
				}

				lhs = make_unique<bin_expr_node>(op, move(lhs), move(rhs));
			}
		}

		int parser::get_tok_prec() {
			if (!isascii(mCurTok->get_lexeme_str()[0])) {
				return -1;
			}

			// Make sure it's a declared binop.
			int tokPrec = mBinOpPrec[mCurTok->get_lexeme_str()[0]];
			if (tokPrec <= 0)
				return -1;

			return tokPrec;
		}

		void parser::expect(const wstring &expected, wstring &actual, compiler_logger::error_level level) const {
			// For debugging purposes
			if (level > 0) {
				assert(expected == actual);
			}

			switch(level) {
			case error_level::ERR_LVL_DEBUG:
				if(mCompileLogger) {
					mCompileLogger->debug(L"Expected: \"" + expected + L"\", got: \"" + actual + L"\"");
				}
			case error_level::ERR_LVL_WARN:
				if (mCompileLogger) {
					mCompileLogger->warn(L"Expected: \"" + expected + L"\", got: \"" + actual + L"\"");
				}
			case error_level::ERR_LVL_ERROR:
				if (mCompileLogger) {
					mCompileLogger->error(L"Expected: \"" + expected + L"\", got: \"" + actual + L"\"");
				}
			}
		}

		void parser::expect(lexer_tok_type expected, lexer_tok_type actual, compiler_logger::error_level level) const {
			// For debugging purposes
			if (level > 0) {
				assert(expected == actual);
			}

			switch (level) {
			case error_level::ERR_LVL_DEBUG:
				if (mCompileLogger) {
					mCompileLogger->debug(L"Expected token: \"" + lexer_token::map_token(expected) +
						L"\", got: \"" + lexer_token::map_token(actual) + L"\"");
				}
			case error_level::ERR_LVL_WARN:
				if (mCompileLogger) {
					mCompileLogger->warn(L"Expected token: \"" + lexer_token::map_token(expected) +
						L"\", got: \"" + lexer_token::map_token(actual) + L"\"");
				}
			case error_level::ERR_LVL_ERROR:
				if (mCompileLogger) {
					mCompileLogger->error(L"Expected token: \"" + lexer_token::map_token(expected) +
						L"\", got: \"" + lexer_token::map_token(actual) + L"\"");
				}
			}
		}
	}
}

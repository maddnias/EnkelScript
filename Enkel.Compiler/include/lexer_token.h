#pragma once
#include <cassert>
#include <string>
#include <unordered_map>
#include "variant_datatype.h"


namespace enkel {
	namespace compiler {
		static std::unordered_map<int, std::wstring> mTokMappings = {
			{ 0, L"TOK_EOF" },
			{ 1, L"TOK_FUNC_DEF" },
			{ 2, L"TOK_END" },
			{ 3, L"TOK_IDENTIFIER" },
			{ 4, L"TOK_NUMBER" },
			{ 5, L"TOK_UNK" },
			{ 6, L"TOK_OPEN_PARENTH" },
			{ 7, L"TOK_CLOSE_PARENTH" },
			{ 8, L"TOK_COMMENT" },
			{ 9, L"TOK_TYPE_DECL" },
			{ 10, L"TOK_VAR_DECL" },
			{ 11, L"TOK_FUNC_TYPE_DECL" },
			{ 12, L"TOK_KEYWORD" },
			{ 13, L"TOK_COMMA" },
			{ 14, L"TOK_ASSIGN" },
			{ 15, L"TOK_EQU" },
			{ 16, L"TOK_OP_UNARY" },
			{ 17, L"TOK_OP_BIN" },
			{ 18, L"TOK_VAR_DECL_SCOPE" },
			{ 19, L"TOK_LITERAL" },
			{ 20, L"TOK_DQUOTE" },
			{ 21, L"TOK_SQUOTE" },
			{ 22, L"TOK_STL_CONST"}
		};

		enum lexer_tok_type : int {
			TOK_EOF,
			TOK_FUNC_DECL,
			TOK_END,
			TOK_IDENTIFIER,
			TOK_INTEGER,
			TOK_UNK,
			TOK_OPEN_PARENTH,
			TOK_CLOSE_PARENTH,
			TOK_COMMENT,
			TOK_FUNC_TYPE_DECL,
			TOK_VAR_DECL,
			TOK_TYPE_DECL,
			TOK_KEYWORD,
			TOK_COMMA,
			TOK_ASSIGN,
			TOK_EQU,
			TOK_OP_UNARY,
			TOK_OP_BIN,
			TOK_VAR_DECL_SCOPE,
			TOK_LITERAL,
			TOK_DQUOTE,
			TOK_SQUOTE,
			TOK_STL_CONST
		};

		class lexer_token {
		public:
			lexer_token();
			lexer_token(const lexer_tok_type &tokType, const std::wstring data,
				const unsigned int col, const unsigned int row);
			~lexer_token();

			const lexer_tok_type& get_type() const;

			runtime::variant_datatype get_lexeme_num() const;

			const std::wstring& get_lexeme_str() const {
				return mLexeme;
			}

			static std::wstring &map_token(lexer_tok_type type) {
				assert(mTokMappings.find(type) != mTokMappings.end());
				return mTokMappings[0];
			}

			std::wstring dump() const;

		private:
			lexer_tok_type mTokType;
			std::wstring mLexeme;
			unsigned int mCol;
			unsigned int mRow;
		};
	}
}

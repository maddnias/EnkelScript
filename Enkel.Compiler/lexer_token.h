#pragma once
#include <string>
#include <unordered_map>
#include <cassert>
#include "variant_datatype.h"


namespace enkel {
	namespace compiler {
		static std::unordered_map<int, std::wstring> mTokMappings = {
			{ 0, _T("TOK_EOF") },
			{ 1, _T("TOK_FUNC_DEF") },
			{ 2, _T("TOK_END") },
			{ 3, _T("TOK_IDENTIFIER") },
			{ 4, _T("TOK_NUMBER") },
			{ 5, _T("TOK_UNK") },
			{ 6, _T("TOK_OPEN_PARENTH") },
			{ 7, _T("TOK_CLOSE_PARENTH") },
			{ 8, _T("TOK_COMMENT") },
			{ 9, _T("TOK_TYPE_DECL") },
			{ 10, _T("TOK_VAR_DECL") },
			{ 11, _T("TOK_FUNC_TYPE_DECL") },
			{ 12, _T("TOK_KEYWORD") },
			{ 13, _T("TOK_COMMA") },
			{ 14, _T("TOK_ASSIGN") },
			{ 15, _T("TOK_EQU") },
			{ 16, _T("TOK_OP_UNARY") },
			{ 17, _T("TOK_OP_BIN") },
			{ 18, _T("TOK_VAR_DECL_SCOPE") },
			{ 19, _T("TOK_LITERAL") },
			{ 20, _T("TOK_DQUOTE") }
		};

		enum lexer_tok_type : int {
			TOK_EOF,
			TOK_FUNC_DECL,
			TOK_END,
			TOK_IDENTIFIER,
			TOK_NUMBER,
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
			TOK_DQUOTE
		};

		class lexer_token {
		public:
			lexer_token();
			lexer_token(const lexer_tok_type &tokType, const std::wstring data,
				const unsigned int col, const unsigned int row);
			~lexer_token();

			const lexer_tok_type& get_type() const;

			runtime::variant_datatype get_lexeme_num() const {
				runtime::variant_datatype data;
				if(mLexeme == L"0") {
					return runtime::variant_datatype(0);
				}
				wchar_t *end;
				uint64_t interp = wcstoull(mLexeme.c_str(), &end, 10);
				//TODO: possible error with negatives
				if(interp <= INT32_MAX) {
					return runtime::variant_datatype(static_cast<int>(interp));
				} 
				if(interp <= INT64_MAX) {
					return runtime::variant_datatype(static_cast<int64_t>(interp));
				}
				//TODO: double
				return runtime::variant_datatype(interp);
			}

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

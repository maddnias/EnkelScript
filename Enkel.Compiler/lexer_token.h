#pragma once
#include <string>
#include <unordered_map>
#include <cassert>

namespace enkel {
	namespace compiler {
		static std::unordered_map<int, std::string> mTokMappings = {
			{ 0, "TOK_EOF" },
			{ 1, "TOK_FUNC_DEF" },
			{ 2, "TOK_END" },
			{ 3, "TOK_IDENTIFIER" },
			{ 4, "TOK_NUMBER" },
			{ 5, "TOK_UNK" },
			{ 6, "TOK_OPEN_PARENTH" },
			{ 7, "TOK_CLOSE_PARENTH" },
			{ 8, "TOK_COMMENT" },
			{ 9, "TOK_TYPE_DECL" },
			{ 10, "TOK_VAR_DECL" },
			{ 11, "TOK_FUNC_TYPE_DECL" },
			{ 12, "TOK_KEYWORD" },
			{ 13, "TOK_COMMA" },
			{ 14, "TOK_ASSIGN" },
			{ 15, "TOK_EQU" },
			{ 16, "TOK_OP_UNARY" },
			{ 17, "TOK_OP_BIN" },
			{ 18, "TOK_VAR_DECL_SCOPE"}
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
			TOK_VAR_DECL_SCOPE
		};

		class lexer_token {
		public:
			lexer_token();
			lexer_token(const lexer_tok_type &tokType, const std::string data,
				const unsigned int col, const unsigned int row);
			~lexer_token();

			const lexer_tok_type& get_type() const;

			const int& get_lexeme_i32() const {
				return stoi(mLexeme);
			}

			const std::string& get_lexeme_str() const {
				return mLexeme;
			}

			static std::string &map_token(lexer_tok_type type) {
				assert(mTokMappings.find(type) != mTokMappings.end());
				return mTokMappings[0];
			}

			std::string dump() const;

		private:
			lexer_tok_type mTokType;
			std::string mLexeme;
			unsigned int mCol;
			unsigned int mRow;
		};
	}
}

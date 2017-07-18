#include "stdafx.h"
#include "lexer_token.h"

using std::to_string;

namespace enkel {
	namespace compiler {
		lexer_token::lexer_token()
			: mTokType(TOK_UNK),
			mCol(0),
			mRow(0) {
		}

		lexer_token::lexer_token(const lexer_tok_type & tokType, const std::string data, const unsigned int col, const unsigned int row)
			: mTokType(tokType),
			mLexeme(data),
			mCol(col),
			mRow(row) {
		}
		lexer_token::~lexer_token() {
		}

		const lexer_tok_type& lexer_token::get_type() const {
			return mTokType;
		}

		std::string lexer_token::dump() const {
			return "TOKEN: " + mTokMappings[mTokType] + ": \"" + mLexeme + "\" @ (" + to_string(mRow)
				+ ":" + to_string(mCol) + ")";
		}
	}
}
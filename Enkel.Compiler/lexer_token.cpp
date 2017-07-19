#include "stdafx.h"
#include "lexer_token.h"


namespace enkel {
	namespace compiler {
		lexer_token::lexer_token()
			: mTokType(TOK_UNK),
			mCol(0),
			mRow(0) {
		}

		lexer_token::lexer_token(const lexer_tok_type & tokType, const std::wstring data,
			const unsigned int col, const unsigned int row)
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

		std::wstring lexer_token::dump() const {
			return L"TOKEN: " + mTokMappings[mTokType] + L": \"" + mLexeme + L"\" @ (" + std::to_wstring(mRow)
				+ L":" + std::to_wstring(mCol) + L")";
		}
	}
}
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

		runtime::variant_datatype lexer_token::get_lexeme_num() const {
			// Check if it's a double
			//TODO: localize separator?
			if (mLexeme.find('.') != std::string::npos) {
				wchar_t *end;
				double dVal = wcstod(mLexeme.c_str(), &end);
				dVal += _wtoi64(std::wstring(end).substr(1).c_str()) / pow(10, std::wstring(end).size()-1);
				return runtime::variant_datatype(dVal);
			}

			if (mLexeme == L"0") {
				return runtime::variant_datatype(0);
			}
			wchar_t *end;
			uint64_t interp = wcstoull(mLexeme.c_str(), &end, 10);
			//TODO: possible error with negatives
			if (interp <= INT32_MAX) {
				return runtime::variant_datatype(static_cast<int>(interp));
			}
			if (interp <= INT64_MAX) {
				return runtime::variant_datatype(static_cast<int64_t>(interp));
			}
			//TODO: double
			return runtime::variant_datatype(interp);
		}

		std::wstring lexer_token::dump() const {
			return L"TOKEN: " + mTokMappings[mTokType] + L": \"" + mLexeme + L"\" @ (" + std::to_wstring(mRow)
				+ L":" + std::to_wstring(mCol) + L")";
		}
	}
}

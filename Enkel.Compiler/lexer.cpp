#include "stdafx.h"
#include "lexer.h"
#include <memory>
#include <cassert>
#include <codecvt>

using namespace std;

#define MAKE_TOK(type, data) make_shared<lexer_token>(type,data,fixedCol,mRow)

namespace enkel {
	namespace compiler {
		lexer::lexer()
			: mCol(0),
			  mRow(0) {
		}

		lexer::~lexer() {
		}

		wchar_t lexer::eat_ws() {
			wchar_t lastChar = peek_next_char();

			while (iswspace(lastChar)) {
				get_next_char();
				lastChar = peek_next_char();
			}

			return lastChar;
		}

		wchar_t lexer::get_next_char() {
			wchar_t next = mData->get();
			//TODO: \r\n
			if (next == '\n') {
				mRow++;
				mCol = 0;
			}
			else if (next == '\r' && peek_next_char() == '\n') {
				mRow++;
				mCol = 0;
				// eat \n
				next = mData->get();
			}
			else {
				mCol++;
			}

			return next;
		}

		wchar_t lexer::peek_next_char() const {
			return mData->peek();
		}

		wchar_t lexer::peek(int count) const {
			streampos curLoc = mData->tellg();
			mData->seekg(curLoc + static_cast<streampos>(count));
			int peekedChar = peek_next_char();
			mData->seekg(curLoc);

			return peekedChar;
		}

		wstring lexer::parse_alpha() {
			wstring parsedDat;
			int lastChar = peek_next_char();

			while (iswalnum(lastChar)) {
				parsedDat += get_next_char();
				// Don't consume last character
				lastChar = peek_next_char();
			}

			return parsedDat;
		}

		wstring lexer::parse_comment() {
			wstring commentData;
			int nextChar = get_next_char();

			while (nextChar != EOF && nextChar != '\n' && nextChar != '\r') {
				commentData += nextChar;
				nextChar = get_next_char();
			}

			return commentData;
		}

		wstring lexer::parse_number() {
			wstring numberData;
			int nextChar = peek_next_char();

			while (isdigit(nextChar)) {
				numberData += get_next_char();
				nextChar = peek_next_char();
			}

			return numberData;
		}

		wstring lexer::parse_string_literal() {
			// Eat '"'
			eat();

			wstring literalData;
			// TODO: real rules
			while(peek_next_char() != '"') {
				wchar_t nextChar = get_next_char();
				// Handle escaped characters
				if(nextChar == '\\') {
					switch(peek_next_char()) {
					case '\'':
						literalData += '\'';
						eat();
						break;
					case '\"':
						literalData += '\"';
						eat();
						break;
					case '\\':
						literalData += '\\';
						eat();
						break;
					case '0':
						literalData += static_cast<wchar_t>('\0');
						eat();
						break;
					case 'r':
						literalData += '\r';
						eat();
						break;
					case 't':
						literalData += '\t';
						eat();
						break;
					case 'n':
						literalData += '\n';
						eat();
						break;
					default:
						literalData += nextChar;
						eat();
						break;
						//TODO: all of them
					}
				} else {
					literalData += nextChar;
				}
			}
			// Eat '"'
			eat();

			return literalData;
		}

		bool lexer::initialize(unique_ptr<wistream> &data) {
			mData = move(data);
			if (!mData) {
				return false;
			}

			locale::global(locale(""));
			mData->imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
			wchar_t bomChar = peek_next_char();

			// Check if BOM is present, if so skip over it
			if(bomChar == 0xFFFE || bomChar == 0xFEFF) {
				get_next_char();
			}

			return true;
		}

		//TODO: make all tolower before parsing? eg LOCAL GLOBAL
		shared_ptr<lexer_token> lexer::next_token() {
			int nextChar = eat_ws();
			int fixedCol = mCol;

			if (iswalpha(nextChar)) {
				wstring dat = parse_alpha();
				mData->seekg(-1, ios::cur);
				// Check if it is a keyword first
				if(find(mKeywords.begin(), mKeywords.end(), dat) != mKeywords.end()) {
					return MAKE_TOK(TOK_KEYWORD, dat);
				}

				if (dat == L"end") {
					return MAKE_TOK(TOK_END, dat);
				}
				if (dat == L"func") {
					return MAKE_TOK(TOK_FUNC_DECL, dat);
				}
				if (dat == L"var") {
					return MAKE_TOK(TOK_VAR_DECL, dat);
				}
				if(dat == L"local" || dat == L"global") {
					return MAKE_TOK(TOK_VAR_DECL_SCOPE, dat);
				}

				return MAKE_TOK(TOK_IDENTIFIER, dat);
			}

			if (iswdigit(nextChar)) {
				return MAKE_TOK(TOK_NUMBER, parse_number());
			}

			if(nextChar == '"') {
				return MAKE_TOK(TOK_LITERAL, parse_string_literal());
			}

			if (nextChar == '(') {
				// Eat parenthesis
				eat();
				return MAKE_TOK(TOK_OPEN_PARENTH, L"(");
			}
			if (nextChar == ')') {
				eat();
				return MAKE_TOK(TOK_CLOSE_PARENTH, L")");
			}

			if (nextChar == '#') {
				return MAKE_TOK(TOK_COMMENT, parse_comment());
			}

			if(nextChar == ':') {
				eat();
				return MAKE_TOK(TOK_TYPE_DECL, L":");
			}

			if(nextChar == ',') {
				eat();
				return MAKE_TOK(TOK_COMMA, L",");
			}

			if (nextChar == '-' && peek(1) == '>') {
				eat(2);
				return MAKE_TOK(TOK_FUNC_TYPE_DECL, L"->");
			}

			switch(nextChar) {
			case '=':
				eat();
				if (peek(1) == '=') {
					eat();
					return MAKE_TOK(TOK_OP_BIN, L"==");
				}
				return MAKE_TOK(TOK_ASSIGN, L"=");
			case '!':
				eat();
				if(peek(1) == '=') {
					eat();
					return MAKE_TOK(TOK_OP_BIN, L"!=");
				}
				return MAKE_TOK(TOK_OP_BIN, L"!");
			case '+':
				eat();
				//TODO: unary
				return MAKE_TOK(TOK_OP_BIN, L"+");
			}

			if(nextChar == '=') {
				eat();
				// = is assignment, == is equality
				if(peek(1) == '=') {
					eat();
					return MAKE_TOK(TOK_EQU, L"==");
				}
				return MAKE_TOK(TOK_ASSIGN, L"=");
			}

			if (nextChar == EOF || nextChar == WEOF) {
				return MAKE_TOK(TOK_EOF, L"EOF");
			}

			return MAKE_TOK(TOK_UNK, wstring(1, static_cast<char>(get_next_char())));
		}

		shared_ptr<lexer_token> lexer::peek_next_token(int count) {
			assert(count > 0);
			streampos curPos = mData->tellg();
			//mData->seekg(-1, ios::cur);
			for (int i = 0; i < count - 1; i++) {
				next_token();
			}
			auto tok = next_token();
			mData->seekg(curPos);

			return tok;
		}

		void lexer::reset() {
			mData->clear();
			mData->seekg(0);
			mCol = 0;
			mRow = 0;
		}

		void lexer::eat(int count) {
			for (int i = 0; i < count; i++) {
				get_next_char();
			}
		}
	}
}

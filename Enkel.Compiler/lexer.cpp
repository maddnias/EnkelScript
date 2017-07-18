#include "stdafx.h"
#include "lexer.h"
#include <memory>
#include <cassert>

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

		int lexer::eat_ws() {
			int lastChar = peek_next_char();

			while (isspace(lastChar)) {
				get_next_char();
				lastChar = peek_next_char();
			}

			return lastChar;
		}

		int lexer::get_next_char() {
			int next = mData->get();
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

		int lexer::peek_next_char() const {
			return mData->peek();
		}

		int lexer::peek(int count) const {
			streampos curLoc = mData->tellg();
			mData->seekg(curLoc + static_cast<streampos>(count));
			int peekedChar = peek_next_char();
			mData->seekg(curLoc);

			return peekedChar;
		}

		string lexer::parse_alpha() {
			string parsedDat;
			int lastChar = peek_next_char();

			while (isalnum(lastChar)) {
				parsedDat += get_next_char();
				// Don't consume last character
				lastChar = peek_next_char();
			}

			return parsedDat;
		}

		string lexer::parse_comment() {
			string commentData;
			int nextChar = get_next_char();

			while (nextChar != EOF && nextChar != '\n' && nextChar != '\r') {
				commentData += nextChar;
				nextChar = get_next_char();
			}

			return commentData;
		}

		string lexer::parse_number() {
			string numberData;
			int nextChar = peek_next_char();

			while (isdigit(nextChar)) {
				numberData += get_next_char();
				nextChar = peek_next_char();
			}

			return numberData;
		}

		bool lexer::initialize(unique_ptr<istream> &data) {
			mData = move(data);
			if (!mData) {
				return false;
			}

			return true;
		}

		//TODO: make all tolower before parsing? eg LOCAL GLOBAL
		shared_ptr<lexer_token> lexer::next_token() {
			int nextChar = eat_ws();
			int fixedCol = mCol;

			if (isalpha(nextChar)) {
				string dat = parse_alpha();
				// Check if it is a keyword first
				if(find(mKeywords.begin(), mKeywords.end(), dat) != mKeywords.end()) {
					return MAKE_TOK(TOK_KEYWORD, dat);
				}

				if (dat == "end") {
					return MAKE_TOK(TOK_END, dat);
				}
				if (dat == "func") {
					return MAKE_TOK(TOK_FUNC_DECL, dat);
				}
				if (dat == "var") {
					return MAKE_TOK(TOK_VAR_DECL, dat);
				}
				if(dat == "local" || dat == "global") {
					return MAKE_TOK(TOK_VAR_DECL_SCOPE, dat);
				}
				return MAKE_TOK(TOK_IDENTIFIER, dat);
			}

			if (isdigit(nextChar)) {
				return MAKE_TOK(TOK_NUMBER, parse_number());
			}

			if (nextChar == '(') {
				// Eat parenthesis
				eat();
				return MAKE_TOK(TOK_OPEN_PARENTH, "(");
			}
			if (nextChar == ')') {
				eat();
				return MAKE_TOK(TOK_CLOSE_PARENTH, ")");
			}

			if (nextChar == '#') {
				return MAKE_TOK(TOK_COMMENT, parse_comment());
			}

			if(nextChar == ':') {
				eat();
				return MAKE_TOK(TOK_TYPE_DECL, ":");
			}

			if(nextChar == ',') {
				eat();
				return MAKE_TOK(TOK_COMMA, ",");
			}

			if (nextChar == '-' && peek(1) == '>') {
				eat(2);
				return MAKE_TOK(TOK_FUNC_TYPE_DECL, "->");
			}

			switch(nextChar) {
			case '=':
				eat();
				if (peek(1) == '=') {
					eat();
					return MAKE_TOK(TOK_OP_BIN, "==");
				}
				return MAKE_TOK(TOK_ASSIGN, "=");
			case '!':
				eat();
				if(peek(1) == '=') {
					eat();
					return MAKE_TOK(TOK_OP_BIN, "!=");
				}
				return MAKE_TOK(TOK_OP_BIN, "!");
			case '+':
				eat();
				//TODO: unary
				return MAKE_TOK(TOK_OP_BIN, "+");
			}

			if(nextChar == '=') {
				eat();
				// = is assignment, == is equality
				if(peek(1) == '=') {
					eat();
					return MAKE_TOK(TOK_EQU, "==");
				}
				return MAKE_TOK(TOK_ASSIGN, "=");
			}

			if (nextChar == EOF) {
				return MAKE_TOK(TOK_EOF, "EOF");
			}

			return MAKE_TOK(TOK_UNK, string(1, static_cast<char>(get_next_char())));
		}

		shared_ptr<lexer_token> lexer::peek_next_token(int count) {
			assert(count > 0);
			streampos curPos = mData->tellg();
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

#pragma once
#include "lexer_token.h"
#include <memory>

#ifdef UNICODE
#define ENKEL_EOF WEOF
#else
#define ENKEL_EOF EOF
#endif

namespace enkel {
	namespace compiler {
		class lexer
		{
		public:
			lexer();
			~lexer();

			/**
			 * \brief Initializes the lexer from an input stream.
			 * \param data The input data to be tokenized
			 * \return Returns true if input stream was successfully loaded
			 */
			bool initialize(std::unique_ptr<std::wistream> &data);
			std::shared_ptr<lexer_token> next_token();
			std::shared_ptr<lexer_token> peek_next_token(int count = 1);
			void reset();

		private:
			void eat(int count = 1);
			wchar_t eat_ws();
			wchar_t get_next_char();
			wchar_t peek_next_char() const;
			wchar_t peek(int count = 0) const;

			std::wstring parse_alpha();
			std::wstring parse_comment();
			std::wstring parse_number();
			std::wstring parse_string_literal();

			std::unique_ptr<std::wistream> mData;

			unsigned int mCol;
			unsigned int mRow;

			std::vector<std::wstring> mKeywords = {
				L"return",
				L"if",
				L"else",
				L"ref"
			};
		};
	}
}

#pragma once
#include <memory>
#include "lexer_token.h"

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
			bool initialize(std::unique_ptr<std::istream> &data);
			std::shared_ptr<lexer_token> next_token();
			std::shared_ptr<lexer_token> peek_next_token(int count = 1);
			void reset();

		private:
			void eat(int count = 1);
			int eat_ws();
			int get_next_char();
			int peek_next_char() const;
			int peek(int count = 0) const;

			std::string parse_alpha();
			std::string parse_comment();
			std::string parse_number();

			std::unique_ptr<std::istream> mData;

			unsigned int mCol;
			unsigned int mRow;

			std::vector<std::string> mKeywords = {
				"return",
				"int",
				"string"
			};
		};
	}
}
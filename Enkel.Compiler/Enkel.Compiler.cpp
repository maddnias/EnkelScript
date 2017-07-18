#include "stdafx.h"
#include <string>
#include "lexer.h"
#include <sstream>
#include <memory>
#include <iostream>

using namespace std;
using namespace enkel::compiler;

int main() {
	lexer l;
	string test =
		"func test(param1 : int, param2 : int) -> int\r\n"
		"return param1+param2\r\n"
		"end";

	unique_ptr<istream> testa(new istringstream(test));

	if (!l.initialize(testa)) {
		cout << "Failed to initialize lexer";
	}

	shared_ptr<lexer_token> tok;
	while ((tok = l.next_token())->get_type() != TOK_EOF) {
		cout << tok->dump() << endl;
	}

	return 0;
}


// Enkel.Bootstrapper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <istream>
#include <sstream>

#include <iostream>
#include "../Enkel.Compiler/enkel_runtime.h"
#include "module_node.h"


std::string test =
"myVar1 = 10\r\n"
"myVar2 = 100\r\n"
"myVar3 = myVar1 + myVar2\r\n"
"print(myVar3)";

int main() {
	std::unique_ptr<std::istream> testa(new std::istringstream(test));

	enkel::runtime::enkel_runtime rt;
	rt.exec(move(testa));

	return 0;
}


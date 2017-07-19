// Enkel.Bootstrapper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <istream>
#include <fstream>
#include <sstream>

#include <iostream>
#include "../Enkel.Compiler/enkel_runtime.h"


std::wstring test =
L"myVar1 = 2147483647\r\n"
"myVar2 = 150\r\n"
"myVar3 = myVar1 + myVar2\r\n"
"print(myVar3)";

int main() {
	std::unique_ptr<std::wistream> testa(new std::wstringstream(test));

	enkel::runtime::enkel_runtime rt;
	rt.exec(move(testa));

	return 0;
}


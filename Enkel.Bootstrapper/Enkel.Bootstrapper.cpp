// Enkel.Bootstrapper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <istream>
#include <fstream>
#include <sstream>

#include <iostream>
#include "../Enkel.Compiler/enkel_runtime.h"
#include "module_node.h"


std::wstring test =
L"myVar1 = 10\r\n"
"myVar2 = \"150\"\r\n"
"myVar3 = myVar1 + myVar2\r\n"
"print(myVar3)";

int main() {
	__int64 lul = LLONG_MAX;
	double lil = LLONG_MAX;

	std::unique_ptr<std::wistream> testa(new std::wifstream("C:\\script.es"));

	enkel::runtime::enkel_runtime rt;
	rt.exec(move(testa));

	return 0;
}


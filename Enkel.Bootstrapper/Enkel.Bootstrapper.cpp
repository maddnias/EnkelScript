// Enkel.Bootstrapper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <istream>
#include <sstream>
#include <fstream>

#include "enkel_runtime.h"
#include <memory>
#include <iostream>
#include <codecvt>

#include <fcntl.h>
#include <io.h>

std::wstring test =
L"myVar1 := 100\n"
"myVar2 := 150\n"
"myVar3 := myVar1 + myVar2\n"
"if myVar2 = 150\n"
"myVar2 := 200\n"
"else if myVar2 = 1000\n"
"myVar2 := 1\n"
"else\n"
"myVar2 := 0\n"
"end\n"
"print(myVar3)";

int main() {
	std::wifstream *instr = new std::wifstream(L"C:\\script.es");
	std::wstringstream *gugugaga = new std::wstringstream();

	_setmode(_fileno(stdout), _O_WTEXT);

	*gugugaga << instr->rdbuf();

	std::unique_ptr<std::wistream> testa(gugugaga);
	//std::unique_ptr<std::wistream> testa(new std::wstringstream(test));

	enkel::runtime::enkel_runtime rt;
	rt.exec(move(testa));

	return 0;
}


// calc.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include "CalcCostTime.h"

void test()
{
	CCostTime obj(L"test");
	Sleep(1000);
}

int main(int argc, char* argv[])
{
	test();
	system("pause");
	return 0;
}


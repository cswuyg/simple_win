// catch_pure_virtual_func_call.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "model/pure_call.h"
#include <windows.h>
#include <stdexcept>
#include "D:\temp\catch_pure_virtual_func_call\catch_pure_dll\catch_pure_dll.h"
#include "model/catch_dump_file.h"
#include <iostream>

void test()
{
	CPureCall obj;
}

CatchDumpFile::CDumpCatch g_exception_handler;

typedef int (*FunType)();
int _tmain(int argc, _TCHAR* argv[])
{
	//test();
	HANDLE hDll  =LoadLibrary(L"D:\\temp\\catch_pure_virtual_func_call\\Debug\\catch_pure_dll.dll");
	if (hDll)
	{
		FunType func =  (FunType)GetProcAddress((HMODULE)hDll,"fncatch_pure_dll");
		std::cout << func() << std::endl;
	}
	system("pause");
	return 0;
}


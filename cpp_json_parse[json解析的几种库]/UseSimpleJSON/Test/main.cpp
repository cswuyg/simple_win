// TestJsonLib.cpp : Defines the entry point for the console application.
//
//2012-12-7.
//An example about how to use SimpleJson lib.

#include <iostream>
#pragma comment(lib, "SimpleJSON.lib")

#include <windows.h>
#include "test\test.h"

void TestFunc()
{
	CTestJSON testObj;
	std::wstring strJson = testObj.GetJsonStr();
	std::wcout << L"Json : " << strJson << std::endl; 
	std::cout << std::endl << std::endl;
	testObj.parseJsonStr(strJson);

	std::cout << "Next" << std::endl << std::endl << std::endl;
	strJson = testObj.AddJsonStr(strJson);
	std::wcout << L"Json : " << strJson << std::endl; 
	std::cout << std::endl << std::endl;
	testObj.parseJsonStr(strJson);

}


int main(int /*argc*/, char* /*argv[]*/)
{
	TestFunc();
	system("pause");
	return 0;
}


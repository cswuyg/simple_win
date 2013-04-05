// Testjsoncpp.cpp : Defines the entry point for the console application.
//

#include "example/example.h"
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <string>
#include <iostream>

/**
jsoncpp的使用测试
如果不知道json串的数据格式，那么需要使用递归分析。

jsoncpp主要优点是，快，比现有的其它库都快。
jsoncpp主要缺点是不支持unicode，
还有，相比SimpleJson，导出接口多，感觉比较复杂。
工作中，建议使用jsoncpp，不支持直接unicode并不是大问题。
simplejson有内存析构坑，用起来需要小心翼翼。
boost property_tree要带上boost库，太庞大。

2012-12-12
*/

void TestFun()
{
	CTestjsoncpp testobj;
	testobj.test_Parse();
	testobj.test_writefile();
	testobj.test_readfile();
}


int _tmain(int argc, _TCHAR* argv[])
{
	TestFun();
	system("pause");
	return 0;
}


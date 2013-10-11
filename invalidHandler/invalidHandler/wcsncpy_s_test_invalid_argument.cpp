/*
A example about wcsncpy_s's invalid parameter.
2012-10-17
**/
#include "stdafx.h"
#include <iostream>
#include <windows.h>

static void InvalidParameterHandler(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t)
{
	throw std::invalid_argument("参数错误");
}

void TestFunc()
{
	wchar_t* lpszDes = new wchar_t[ 20 ];
	wchar_t lpszSrc[] = L"123456";
	//因为目标buffer要包含字符串结束符，所以目标buffer长度参数必须比源字符串长度大1
	//如果源buffer加上字符串结束符的长度小于等于参数DestLength，
	//那么，即便参数SourLength大于DestLength，也是没问题的，
	//也就是说，参数安全检查，在根据源、目标的length做检查之前，会先检测字符串结束符
	//只要目标buffer长度能容下源字符串，那么就忽略掉源字符串的长度参数检查。
	//字符串安全检测，如果了异常，那么SEH\C++的异常扑捉机制都没法扑捉，只能是通过
	//_set_invalid_parameter_handler的设置函数去扑捉。这个扑捉函数是进程级别的。
	try
	{	
		wcsncpy_s( lpszDes, 5, lpszSrc, 4 ); 
	}
	catch ( std::invalid_argument& err)
	{
		std::cout << err.what() << std::endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	_set_invalid_parameter_handler( InvalidParameterHandler );
	TestFunc();
	system( "pause" );
	return 0;
}


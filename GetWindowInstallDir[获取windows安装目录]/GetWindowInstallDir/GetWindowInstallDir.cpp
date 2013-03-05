// GetWindowInstallDir.cpp : Defines the entry point for the console application.
//
/**
获取windows安装目录
2013.1.29
*/
#include <iostream>
#include <string>
using namespace std;
#include <Windows.h>

std::wstring GetWindowDir()
{
	std::wstring strRet;
	UINT nSize = ::GetWindowsDirectory(NULL, 0);
	wchar_t* lpszPath = new(std::nothrow)wchar_t[nSize+1];
	::memset(lpszPath, 0, nSize+1);
	if (lpszPath != NULL)
	{
		if(::GetWindowsDirectory(lpszPath, nSize+1) != 0)
		{
			std::wcout << L"The windows dir is " << lpszPath << std::endl;
			strRet = lpszPath;
		}
	}
	delete [] lpszPath;
	return strRet;
}

int main(int argc, char* argv[])
{
	std::wstring strPath = GetWindowDir();
	system("pause");
	return 0;
}


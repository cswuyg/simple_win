// getappdata.cpp : Defines the entry point for the console application.
//

//get the appdata path
//cswuyg@gmail

#include <iostream>
#include <string>
#include <windows.h>
#include <ShlObj.h>

std::wstring getAppFolder()
{
	std::wstring strRet;
	TCHAR	szPath[MAX_PATH];
	HRESULT hr = ::SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath);
	if(SUCCEEDED(hr))
	{
		strRet = szPath;
	}
	return strRet;
}



int main(int argc, char* argv[])
{
	std::wcout << getAppFolder() << std::endl;
	system("pause");
	return 0;
}


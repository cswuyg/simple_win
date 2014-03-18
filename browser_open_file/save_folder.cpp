/**
test select folder path
cswuyg@gmail.com
2014.3.10
*/
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <shlobj.h>

void test()
{
	wchar_t path_buffer[MAX_PATH] = { 0 }; 
	BROWSEINFO bi; 
	ZeroMemory(&bi, sizeof(BROWSEINFO)); 
	bi.hwndOwner = NULL; 
	bi.pszDisplayName = path_buffer; 
	bi.lpszTitle = L"Choose a Folder";
	bi.ulFlags = BIF_BROWSEFILEJUNCTIONS; 
	LPITEMIDLIST idl = ::SHBrowseForFolder(&bi); 
	if (idl == NULL) 
	{ 
		std::cout << "error" << std::endl;
		return; 
	} 
	else
	{
		if (::SHGetPathFromIDList(idl, path_buffer))
		{
			std::wcout << path_buffer << std::endl;
		}
		else
		{
			std::cout << "error" << std::endl;
		}
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	test();
	system("pause");
	return 0;
}


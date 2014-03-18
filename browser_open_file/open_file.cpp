/**
testing GetOpenFileName API
cswuyg@gmail.com
*/

#include "stdafx.h"
#include <windows.h>
#include <commdlg.h>
#include <iostream>

void test()
{
    OPENFILENAME ofn;
    wchar_t szFile[MAX_PATH];
    ZeroMemory(&ofn,sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = TEXT('\0');
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("EXE(*.exe)\0*.exe\0RAR(*.rar;*.7z;*.gz)\0*.rar;*.7z;*.gz\0All File(*.*)\0*.*\0\0"); 
    ofn.nFilterIndex = 1;    
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if(GetOpenFileName(&ofn))
	{
		std::wcout << szFile << std::endl;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	test();
	system("pause");
	return 0;
}


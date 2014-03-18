/**
Check A File is PE File.
cswuyg@gmail.com
2014.3.10
*/
#include "stdafx.h"
#include <windows.h>
#include <string>

enum E_PE_Type
{
	pe_other,
	pe_EXE,
	pe_DLL,
};

E_PE_Type WINAPI IsPEFile(HANDLE hFile)  
{  
	DWORD red = 0;
	DWORD file_size = ::GetFileSize(hFile, NULL); 
	if (file_size > (sizeof(IMAGE_DOS_HEADER) + sizeof(IMAGE_NT_HEADERS)))  //PE文件size至少要大于DOS头+NT头
	{ 
		::SetFilePointer(hFile, 0, NULL, FILE_BEGIN); 
		unsigned short magic; 
		::ReadFile(hFile, &magic, sizeof(magic), &red, NULL);
		if (magic == IMAGE_DOS_SIGNATURE) //DOS 头部
		{
			::SetFilePointer(hFile, (DWORD)&((IMAGE_DOS_HEADER*)0)->e_lfanew, NULL, FILE_BEGIN);   //0x3C 指出NT HEADER偏移
			unsigned long nt_head_pos;  
			::ReadFile(hFile, &nt_head_pos, sizeof(nt_head_pos), &red, NULL);
			if (file_size > nt_head_pos + sizeof(IMAGE_NT_HEADERS))
			{
				::SetFilePointer(hFile, nt_head_pos, NULL, FILE_BEGIN); 
				IMAGE_NT_HEADERS nt_header = { 0 }; 
				::ReadFile(hFile, &nt_header, sizeof(IMAGE_NT_HEADERS), &red, NULL);
				if (nt_header.Signature == IMAGE_NT_SIGNATURE)  //有PE标识
				{
					if (nt_header.FileHeader.Characteristics & IMAGE_FILE_DLL)  //DLL标识
					{
						return pe_DLL;
					}
					else if (nt_header.FileHeader.Characteristics & IMAGE_FILE_EXECUTABLE_IMAGE)  //EXE标识
					{
						return pe_EXE;
					}
				}
			}
		}
	}
	return pe_other;
}

void test3()
{
	std::wstring file_path = L"D:\\test.dll";
	HANDLE hFile = ::CreateFile(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	IsPEFile(hFile);
	::CloseHandle(hFile);
}

int _tmain(int argc, _TCHAR* argv[])
{
	test3();
	system("pause");
	return 0;
}

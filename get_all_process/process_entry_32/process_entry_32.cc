//http://www.pediy.com/kssd/pediy04/kanxue036.htm
//本例子，可能因为权限问题无法获取父进程为explorer.exe的父进程名

#include <windows.h> 
#include <tlhelp32.h> 
#include <psapi.h> 
#include <stdio.h> 
#include <iostream>

#pragma comment(lib, "psapi") 

void DisplayProcessInfo(DWORD pid) 
{ 
	::printf("PID = %8u ", pid); 
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); 
	wchar_t FileName[MAX_PATH]; 
	if (hProcess) 
	{ 
		if (::GetModuleFileNameEx(hProcess, NULL, FileName, MAX_PATH)) 
		{ 
			 ::printf("%ws\n", FileName); 
		} 
		else 
		{ 
			::printf("无法获得进程名\n");
		} 
		::CloseHandle(hProcess); 
	} 
	else 
	{ 
		::printf("无权访问该进程\n");
	} 
} 

void main(void) 
{ 
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
	if (hProcessSnap == INVALID_HANDLE_VALUE) 
	{ 
		::printf("无法创建进程的快照\n");
		return; 
	} 

	PROCESSENTRY32 pe32      = {0}; 
	pe32.dwSize = sizeof(PROCESSENTRY32); 
	if (!Process32First(hProcessSnap, &pe32)) 
	{ 
		::printf("无法获得进程列表\n");
		::CloseHandle(hProcessSnap);
		return; 
	} 

	do 
	{ 
		DisplayProcessInfo(pe32.th32ProcessID); 
		DisplayProcessInfo(pe32.th32ParentProcessID); 
		::printf("\n");
		::printf("\n");
	} while(::Process32Next(hProcessSnap, &pe32)); 
	::CloseHandle(hProcessSnap); 
	::system("pause");
}
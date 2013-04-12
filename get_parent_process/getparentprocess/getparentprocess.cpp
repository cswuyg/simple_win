// getparentprocess.cpp : Defines the entry point for the console application.
//

#include <tchar.h>
#include <windows.h>
#include <winternl.h>
#include <iostream>
#include <string>

std::wstring GetParentProcessFullName()
{
	DWORD parent_pid = 0;
	std::wstring process_name = L"";
	typedef NTSTATUS (WINAPI *NTQUERYINFORMATIONPROCESS)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
	NTQUERYINFORMATIONPROCESS pfn = NULL;
	HMODULE mod = ::GetModuleHandle(L"ntdll.dll");
	pfn = (NTQUERYINFORMATIONPROCESS)::GetProcAddress(mod, "NtQueryInformationProcess");
	if (pfn)
	{
		DWORD dwSizeNeeded = 0;
		PROCESS_BASIC_INFORMATION PBI = {0};
		NTSTATUS dwStatus = pfn(::GetCurrentProcess(), 
			ProcessBasicInformation, &PBI, sizeof(PROCESS_BASIC_INFORMATION), &dwSizeNeeded);
		if (dwStatus == 0)
		{
			parent_pid = (DWORD)PBI.Reserved3;
		}

		HANDLE hParentProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, parent_pid);
		if (hParentProcess)
		{
			CHAR szBuf[512] ;
			::memset(szBuf, 0, sizeof(szBuf));
			LONG status = pfn( hParentProcess, (PROCESSINFOCLASS)27, (PVOID)szBuf, sizeof(szBuf), NULL) ;
			if (NT_SUCCESS(status))
			{
				PUNICODE_STRING lpuImageFileName = (PUNICODE_STRING)szBuf ;
				process_name = lpuImageFileName->Buffer;
			}
			::CloseHandle(hParentProcess) ;
		}
	}
	return process_name;
}


int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring parent_process = GetParentProcessFullName();
	std::wcout << L"Parent Process Name = " << parent_process << std::endl;
	::system("pause");
	return 0;
}


/**
Use SetUnhandledExceptionFilter to catch exception, and use MiniDumpWriteDump get dump file.
because the CRT function may call the SetUnhandledExceptionFilter, and sometimes our handler will no be call, so we must hook the SetUnhandledExceptionFilter.

cswuyg@gmail.com
2013.7.21
*/
#pragma once
#include <windows.h>
#include <Dbghelp.h>
#include <shlwapi.h>
#include <string>
using namespace std; 

#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "Shlwapi.lib")

namespace CatchDumpFile 
{

	void simple_log(const std::wstring& log_msg);
		
	class CDumpCatch
	{
	public:
		CDumpCatch();
		~CDumpCatch();
	private:

		static LPTOP_LEVEL_EXCEPTION_FILTER WINAPI TempSetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);
		static BOOL ReleaseDumpFile(const std::wstring& strPath, EXCEPTION_POINTERS *pException);
		static LONG WINAPI UnhandledExceptionFilterEx(struct _EXCEPTION_POINTERS *pException);

		BOOL AddExceptionHandle();
		BOOL RemoveExceptionHandle();
		BOOL PreventSetUnhandledExceptionFilter();
	private:
		LPTOP_LEVEL_EXCEPTION_FILTER m_preFilter;
	};
};

#include "stdafx.h"
#include "service_control/service_control.h"

const wchar_t* const kServiceName = L"cswuyg_wuyinguang1g1";
const wchar_t* const kBinPath = L"D:\\temp\\my_service_example\\output\\Debug\\bin\\service_example.exe";


std::wstring GetFilePath( const std::wstring& wsFullName )
{
	std::wstring::size_type nIndex1 = wsFullName.find_last_of(L"\\");
	std::wstring::size_type nIndex2 = wsFullName.find_last_of(L"/");
	if (std::wstring::npos == nIndex1)
	{
		nIndex1 = 0;
	}
	if (std::wstring::npos == nIndex2)
	{
		nIndex2 = 0;
	}
	std::wstring::size_type nIndex = max(nIndex1, nIndex2);
	return wsFullName.substr(0, nIndex);
}

std::wstring GetAppPath()
{
	wchar_t lpszFileName[MAX_PATH] = {0};
	::GetModuleFileName(NULL, lpszFileName, MAX_PATH);
	std::wstring strFullName = lpszFileName;

	return GetFilePath(strFullName);
}


void test_service()
{
	ServiceControl control;
	std::wstring app_path = GetAppPath();
	app_path += L"\\service_example.exe";
	control.InstallService(app_path, kServiceName);
	control.UpdateSvcDesc(kServiceName, L"This cswuyg's service, no harm.");
	control.StartService(kServiceName);
	control.AutoStart(kServiceName);
	::Sleep(30000000);
	control.StopService(kServiceName, FALSE);
	control.DeleteService(kServiceName);
}

int _tmain(int argc, _TCHAR* argv[])
{
	test_service();
	::system("pause");
	return 0;
}

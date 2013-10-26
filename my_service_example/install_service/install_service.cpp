#include "stdafx.h"
#include "service_control/service_control.h"

const wchar_t* const kServiceName = L"cswuyg_service";
const wchar_t* const kBinPath = L"D:\\temp\\my_service_example\\output\\Debug\\bin\\service_example.exe";

void test_service()
{
	ServiceControl control;
	control.InstallService(kBinPath, kServiceName);
	control.StartService(kServiceName);
	::Sleep(10000000);
	control.StopService(kServiceName, FALSE);
	control.DeleteService(kServiceName);
}

int _tmain(int argc, _TCHAR* argv[])
{
	test_service();
	::system("pause");
	return 0;
}

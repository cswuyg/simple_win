// service_example.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "service\service_gui.h"



#ifdef _DEBUG
#pragma comment(lib, "libGUIDd.lib")
#pragma comment(lib, "LibDataReportd.lib")
#else
#pragma comment(lib, "libGUID.lib")
#pragma comment(lib, "LibDataReport.lib")
#endif

#include "guid.h"
#include "IDataReport.h"
#include "report_head_lib.h"



BOOL service_test()
{
	ServiceWithGUI obj;
	::OutputDebugString(L"service, service_test"); 
	obj.StartServiceWithGUI(L"D:\\a.exe");
	return TRUE;
};

int _tmain(int argc, _TCHAR* argv[])
{
	service_test();
	return 0;
}


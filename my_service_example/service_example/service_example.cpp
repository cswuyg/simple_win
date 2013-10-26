// service_example.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "service\service_gui.h"

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


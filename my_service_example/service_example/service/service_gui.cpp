#include "stdafx.h"
#include "service_gui.h"
#include "start_gui.h"


#define __WFUNC__ TEXT(__FUNCTION__)

SERVICE_STATUS_HANDLE  ServiceWithGUI::service_status_handle_= 0;
std::wstring ServiceWithGUI::bin_path_;


ServiceWithGUI::ServiceWithGUI()
{
}

ServiceWithGUI::~ServiceWithGUI()
{
}

BOOL ServiceWithGUI::StartServiceWithGUI(const std::wstring& bin_path)
{
	bin_path_ = bin_path;
   SERVICE_TABLE_ENTRY ServiceTable[2];
   ServiceTable[0].lpServiceName = L"";
   ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
   ServiceTable[1].lpServiceName = NULL;
   ServiceTable[1].lpServiceProc = NULL;
	::OutputDebugString(L"service, StartServiceWithGUI"); 
	if (!::StartServiceCtrlDispatcher(ServiceTable)) 
	{ 
		return FALSE;
	} 

	return TRUE;
}

VOID WINAPI ServiceWithGUI::ServiceMain( DWORD argc, LPTSTR *argv )
{
	::OutputDebugString(L"service, StartServiceWithGUI _ 1"); 
	SERVICE_STATUS service_status = { 0 };
	service_status.dwServiceType = SERVICE_WIN32; 
	service_status.dwCurrentState = SERVICE_START_PENDING;
	service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
	service_status.dwWin32ExitCode = 0; 
	service_status.dwServiceSpecificExitCode = 0; 
	service_status.dwCheckPoint = 0; 
	service_status.dwWaitHint = 0; 
	service_status_handle_= ::RegisterServiceCtrlHandler(L"", ServiceCtrlHandler);
	if (service_status_handle_ == 0) 
	{ 
		return; 
	}  
	::OutputDebugString(L"service, StartServiceWithGUI _ 2"); 
	StartGUIProc obj(bin_path_);
	obj.Run();
	service_status.dwCurrentState = SERVICE_RUNNING; 
	service_status.dwCheckPoint = 0; 
	service_status.dwWaitHint = 0; 
	if (!::SetServiceStatus(service_status_handle_, &service_status)) 
	{ 
		return;
	} 
	return;
}

VOID WINAPI ServiceWithGUI::ServiceCtrlHandler( DWORD Opcode )
{
	SERVICE_STATUS service_status = { 0 };
	switch(Opcode) 
	{ 
	case SERVICE_CONTROL_PAUSE: 
		// 完成相关功能
		service_status.dwCurrentState = SERVICE_PAUSED; 
		break; 
	case SERVICE_CONTROL_CONTINUE: 
		// 完成相关功能
		service_status.dwCurrentState = SERVICE_RUNNING; 
		break; 
	case SERVICE_CONTROL_STOP: 
		// 完成相关功能
		service_status.dwWin32ExitCode = 0; 
		service_status.dwCurrentState  = SERVICE_STOPPED; 
		service_status.dwCheckPoint    = 0; 
		service_status.dwWaitHint      = 0; 
		::SetServiceStatus(service_status_handle_, &service_status);
		return; 
	case SERVICE_CONTROL_INTERROGATE: 
		break; 
	default:
		break;
	} 
	// 当前状态
	::SetServiceStatus(service_status_handle_, &service_status);
	return;
}



#include "stdafx.h"
#include "service_gui.h"
#include "start_gui.h"


#define __WFUNC__ TEXT(__FUNCTION__)

SERVICE_STATUS_HANDLE  ServiceWithGUI::service_status_handle_= 0;
std::wstring ServiceWithGUI::bin_path_;
SERVICE_STATUS ServiceWithGUI::service_status_;


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
	ServiceTable[0].lpServiceName = L"My Simple Proc Simpe Service";
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	::OutputDebugString(L"service, StartServiceWithGUI"); 
	if (!::StartServiceCtrlDispatcher(ServiceTable)) 
	{ 
		::OutputDebugString(L"service, Service Proc Main Error End"); 
		return FALSE;
	} 

	::OutputDebugString(L"service, Service Proc Main Suc End"); 
	return TRUE;
}

VOID WINAPI ServiceWithGUI::ServiceMain( DWORD argc, LPTSTR *argv )
{
	::OutputDebugString(L"service, Start Service Main Func"); 
	service_status_handle_= ::RegisterServiceCtrlHandler(L"", ServiceCtrlHandler);
	if (service_status_handle_ == 0) 
	{ 
		return; 
	}  
	service_status_.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
	service_status_.dwServiceSpecificExitCode = 0; 
	ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

	::OutputDebugString(L"service, Start Service Main Func Start GUIDProc"); 
	StartGUIProc obj(bin_path_);
	obj.Run();

	ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);

	while (service_status_.dwCurrentState != SERVICE_STOP_PENDING)
	{
		//to do some jobs
		Sleep(500);
	}
	ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
	return;
}

VOID WINAPI ServiceWithGUI::ServiceCtrlHandler( DWORD Opcode )
{
	switch(Opcode) 
	{ 
	case SERVICE_CONTROL_PAUSE: 
		// 完成相关功能
		::OutputDebugString(L"service, pause"); 
		ReportSvcStatus(SERVICE_PAUSED, NO_ERROR, 0);
		break; 
	case SERVICE_CONTROL_CONTINUE: 
		// 完成相关功能
		::OutputDebugString(L"service, continue"); 
		ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);
		break; 
	case SERVICE_CONTROL_STOP: 
		// 完成相关功能
		::OutputDebugString(L"service, stop"); 
		ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
		break;
	case SERVICE_CONTROL_INTERROGATE: 
		break; 
	default:
		break;
	} 
	return;
}

VOID ServiceWithGUI::ReportSvcStatus( DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint )
{
	static DWORD dwCheckPoint = 1;

	// Fill in the SERVICE_STATUS structure.

	service_status_.dwCurrentState = dwCurrentState;
	service_status_.dwWin32ExitCode = dwWin32ExitCode;
	service_status_.dwWaitHint = dwWaitHint;

	if (dwCurrentState == SERVICE_START_PENDING)
		service_status_.dwControlsAccepted = 0;
	else service_status_.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;

	if ( (dwCurrentState == SERVICE_RUNNING) ||
		(dwCurrentState == SERVICE_STOPPED) )
		service_status_.dwCheckPoint = 0;
	else service_status_.dwCheckPoint = dwCheckPoint++;

	// Report the status of the service to the SCM.
	::SetServiceStatus(service_status_handle_, &service_status_);
}






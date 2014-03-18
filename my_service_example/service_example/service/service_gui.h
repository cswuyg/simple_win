/**
service application example
cswuyg@gmail.com
2013.10.26
&
2014.3.18

some msdn resource
http://msdn.microsoft.com/en-us/library/bb540476(v=vs.85).aspx

*/
#pragma once

#include <string>

class ServiceWithGUI
{
public:
	ServiceWithGUI();
	~ServiceWithGUI();

	BOOL StartServiceWithGUI(const std::wstring& bin_path);

private:
	VOID static WINAPI ServiceMain(DWORD argc, LPTSTR *argv); 
	VOID static WINAPI ServiceCtrlHandler(DWORD Opcode);
	VOID static ReportSvcStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);


private:
	static SERVICE_STATUS_HANDLE service_status_handle_; 
	static std::wstring bin_path_;
	static SERVICE_STATUS service_status_;
};


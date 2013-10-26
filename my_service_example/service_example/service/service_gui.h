/**
service application example
cswuyg@gmail.com
2013.10.26

*/
#pragma once


class ServiceWithGUI
{
public:
	ServiceWithGUI();
	~ServiceWithGUI();

	BOOL StartServiceWithGUI(const std::wstring& bin_path);
	VOID static WINAPI ServiceMain(DWORD argc, LPTSTR *argv); 
	VOID static WINAPI ServiceCtrlHandler(DWORD Opcode);

private:
	static SERVICE_STATUS_HANDLE service_status_handle_; 
	static std::wstring bin_path_;
};


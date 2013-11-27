#pragma once


#include "common\pipe_protocol.h"

#include <windows.h>
#include <string>

class PipClientMgr
{
public:
	PipClientMgr()
		: HasRegister_(FALSE)
	{};
	~PipClientMgr(){};

	BOOL Register();

private:
	HANDLE ConnectToServer();;
	BOOL RegisterClient(HANDLE pipe);
	HANDLE ConnectToPipe(const std::wstring& pipe_name, DWORD pipe_access, DWORD flags_attrs);
	BOOL ValidateResponse(const PipeMsg& reply);
private:
	BOOL HasRegister_;
};
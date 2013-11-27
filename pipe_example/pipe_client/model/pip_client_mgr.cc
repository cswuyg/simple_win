#include "stdafx.h"
#include "pip_client_mgr.h"
#include "common\common.h"

#include <assert.h>

const int kPipeConnectMaxAttempts = 2;
const int kPipeBusyWaitTimeoutMs = 2000;
const DWORD kPipeDesiredAccess = FILE_READ_DATA |
                                 FILE_WRITE_DATA |
                                 FILE_WRITE_ATTRIBUTES;

const DWORD kPipeFlagsAndAttributes = SECURITY_IDENTIFICATION |
                                      SECURITY_SQOS_PRESENT;


const DWORD kPipeMode = PIPE_READMODE_MESSAGE;


HANDLE PipClientMgr::ConnectToServer()
{
	HANDLE pipe = ConnectToPipe(kPipeName, kPipeDesiredAccess, kPipeFlagsAndAttributes);
	if (pipe == NULL)
	{
		return NULL;
	}
	DWORD mode = kPipeMode;
	if (!::SetNamedPipeHandleState(pipe, &mode, NULL, NULL)) 
	{
		::CloseHandle(pipe);
		pipe = NULL;
	}
	return pipe;
}

BOOL PipClientMgr::RegisterClient( HANDLE pipe )
{
	DWORD proc_id = ::GetCurrentProcessId();
	PipeMsg msg = {proc_id, NULL};
	PipeMsg reply;
	DWORD bytes_count = 0;

	if (!::TransactNamedPipe(pipe, &msg, sizeof(msg), &reply, sizeof(PipeMsg), &bytes_count, NULL)) 
	{
		return FALSE;
	}
	if (!ValidateResponse(reply))
	{
		return FALSE;
	}

  return TRUE;
}

HANDLE PipClientMgr::ConnectToPipe( const std::wstring& pipe_name, DWORD pipe_access, DWORD flags_attrs )
{
	for (int i = 0; i < kPipeConnectMaxAttempts; ++i)
	{
		HANDLE pipe = ::CreateFile(pipe_name.c_str(), pipe_access, 0, NULL, OPEN_EXISTING, flags_attrs, NULL);
		if (pipe != INVALID_HANDLE_VALUE)
		{
			return pipe;
		}
		if (::GetLastError() != ERROR_PIPE_BUSY) 
		{
			break;
		}
		if (!::WaitNamedPipe(pipe_name.c_str(), kPipeBusyWaitTimeoutMs)) 
		{
			break;
		}
	}

  return NULL;
}

BOOL PipClientMgr::ValidateResponse( const PipeMsg& /*reply*/ )
{
	return TRUE;
}

BOOL PipClientMgr::Register()
{
	if (HasRegister_)
	{
		assert(FALSE);
		return TRUE;
	}
	HANDLE pipe = ConnectToServer();
	if (pipe == NULL) 
	{
		return FALSE;
	}
	if (RegisterClient(pipe))
	{
		HasRegister_ = TRUE;
	}
	::CloseHandle(pipe);
	return HasRegister_;
}

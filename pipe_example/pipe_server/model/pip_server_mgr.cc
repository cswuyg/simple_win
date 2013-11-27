/**
pipe server manager
cswuyg@gmail.com
2013.11.19
*/

#include "stdafx.h"
#include "pip_server_mgr.h"
#include "common/common.h"
#include <assert.h>

// Attribute flags for the pipe.
const DWORD kPipeAttr = FILE_FLAG_FIRST_PIPE_INSTANCE | PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED;
// Mode for the pipe.
const DWORD kPipeMode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT;
// Output buffer size.
const size_t kOutBufferSize = 64;
// Input buffer size.
const size_t kInBufferSize = 64;

PipServerMgr::PipServerMgr() 
	: pipe_(NULL)
{
}

PipServerMgr::~PipServerMgr()
{
	if (pipe_ != NULL)
	{
		::CloseHandle(pipe_);
		pipe_ = NULL;
	}
}

BOOL PipServerMgr::Start()
{
	pipe_ = ::CreateNamedPipe(kPipeName, kPipeAttr, kPipeMode, 1, kOutBufferSize, kInBufferSize, 0, NULL);
	if (pipe_ == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	HANDLE client_event = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE current_proc = ::GetCurrentProcess();

	while(TRUE)
	{
		BOOL success = ::ConnectNamedPipe(pipe_, NULL);
		if (success)
		{
			PipeMsg pip_msg;
			DWORD read_byte = 0;
			BOOL suc = ::ReadFile(pipe_, &pip_msg, sizeof(PipeMsg), &read_byte, NULL);
			if (suc)
			{
				client_infos_.push_back(pip_msg);
			}
			else
			{
				//error
				break;
			}
			HANDLE client_handle = ::OpenProcess(GENERIC_ALL, FALSE, pip_msg.client_proc_id);
			HANDLE tag_handle = NULL;
			suc = ::DuplicateHandle(current_proc, client_event , client_handle, &tag_handle, EVENT_MODIFY_STATE, FALSE, 0);      
			if (!suc)
			{
				//error
				break;
			}
			PipeMsg pip_reply = {0, tag_handle};
			DWORD bytes_count;
			suc = ::WriteFile(pipe_, &pip_reply, sizeof(pip_reply), &bytes_count, NULL);
			if (!suc)
			{
				//error
				break;
			}
			::DisconnectNamedPipe(pipe_);
		}
	}

	return TRUE;
}

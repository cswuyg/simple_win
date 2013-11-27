#pragma once

#include <windows.h>

struct PipeMsg
{
	DWORD client_proc_id;
	HANDLE event_handle;
};


/**
pipe server manager
cswuyg@gmail.com
2013.11.19
*/

#pragma once

#include "common/pipe_protocol.h"

#include <windows.h>
#include <vector>

class PipServerMgr
{
public:
	PipServerMgr();
	~PipServerMgr();

	BOOL Start();
private:
	HANDLE pipe_;
	std::vector<PipeMsg> client_infos_;
};
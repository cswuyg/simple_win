// pipe_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "model/pip_server_mgr.h"


void test()
{
	PipServerMgr server_mgr;
	server_mgr.Start();
}


int _tmain(int argc, _TCHAR* argv[])
{
	test();
	return 0;
}


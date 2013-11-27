#include "stdafx.h"
#include "model\pip_client_mgr.h"


#include <windows.h>


void test()
{
	PipClientMgr pip_client;
	pip_client.Register();
}

int _tmain(int argc, _TCHAR* argv[])
{
	test();
	::system("pause");
	return 0;
}

#include <iostream>
#include <windows.h>
#include <process.h>
//2013.3.5
//cswuyg
//参考 chromium源码:D:\bd_player3\src\base\threading\platform_thread_win.cc

// The information on how to set the thread name comes from
// a MSDN article: http://msdn2.microsoft.com/en-us/library/xcb2z8hs.aspx
const DWORD kVCThreadNameException = 0x406D1388;

typedef struct tagTHREADNAME_INFO {
	DWORD dwType;  // Must be 0x1000.
	LPCSTR szName;  // Pointer to name (in user addr space).
	DWORD dwThreadID;  // Thread ID (-1=caller thread).
	DWORD dwFlags;  // Reserved for future use, must be zero.
} THREADNAME_INFO;

// This function has try handling, so it is separated out of its caller.
void SetNameInternal(DWORD thread_id, const char* name)
{
	//只在调试的时候生效 
	if (!::IsDebuggerPresent()) 
		return;
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name;
	info.dwThreadID = thread_id;
	info.dwFlags = 0;

	__try
	{
		RaiseException(kVCThreadNameException, 0, sizeof(info)/sizeof(DWORD), reinterpret_cast<DWORD_PTR*>(&info));
	}
	__except(EXCEPTION_CONTINUE_EXECUTION) 
	{
	}
}

static unsigned int _stdcall Run(void* argv)
{
	while(true)
	{
		std::cout << "Thread " << (int)argv << std::endl;
		Sleep(5000);
	}
}

void testFunc()
{
	DWORD thread_id_1;
	HANDLE thread_handle_1 = (HANDLE)_beginthreadex( NULL, 0, Run, (void*)1, 0, (unsigned int*)&thread_id_1);
	SetNameInternal(thread_id_1, "My Thead Thread_1");
	DWORD thread_id_2;
	HANDLE thread_handle_2 = (HANDLE)_beginthreadex( NULL, 0, Run, (void*)2, 0, (unsigned int*)&thread_id_2);
	SetNameInternal(thread_id_2, "My Thead Thread_2");
}


int main()
{
	testFunc();
	system("pause");
	return 1;
}






#include <windows.h>
#include <iostream>
#include <process.h>

/**
1、可等待计时器，可以让创建定时器的线程自己等待，也可让其它多个线程等待。
*/
static unsigned int _stdcall ThreadProc(void* argv)
{
	HANDLE hKernelTimer = static_cast<HANDLE>(argv);
	if(::WaitForSingleObject(hKernelTimer, INFINITE) != WAIT_OBJECT_0)
	{
		return -1;
	}
	std::wcout << "Waitable Timer arrive" << std::endl;
	return 0;
}

/**
异步过程调用，仅当SetWaitableTimer的调用线程处于可提醒状态时，这个ACP函数才会被同一个线程调用
所以必须注意到调用线程使用了SleepEx，其第二个参数是TRUE，当可等待计时器触发ACP调用时，导致睡眠结束。
*/
void static _stdcall TimerACPRoutine(void* pvArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
{
	UNREFERENCED_PARAMETER(dwTimerLowValue);
	UNREFERENCED_PARAMETER(dwTimerHighValue);
	int arg = *(static_cast< int* >(pvArg));
	std::wcout << arg << std::endl;
	//::SleepEx(10000,TRUE); //再让调用线程睡眠
}


/**设置可等待计时器的函数*/
int UseKernelTimer()
{

	HANDLE hKernelTimer = ::CreateWaitableTimer(NULL, FALSE, NULL);
	if (!hKernelTimer)
	{
		return -1;
	}
	const int nTimerUnitsPerSecond = 10000000;
	LARGE_INTEGER li;
	li.QuadPart = -(2 * nTimerUnitsPerSecond);
	int arg = 10;
	if(!::SetWaitableTimer(hKernelTimer, &li, 2000, TimerACPRoutine, &arg, TRUE))
	{
		::CloseHandle(hKernelTimer);
		return -1;
	}
	::_beginthreadex(NULL, NULL, &ThreadProc, hKernelTimer, NULL, NULL);
	//if (::WaitForSingleObject(hKernelTimer, INFINITE) != WAIT_OBJECT_0)
	//{
	//	::CloseHandle(hKernelTimer);
	//	return -1;
	//}
	//::CloseHandle(hKernelTimer);
	//std::wcout << "KernelTimer Arrive" << std::endl;
	return 0;
}



int main()
{
	UseKernelTimer();
	::SleepEx(100000, TRUE);
	::system("pause");
	return 1;
}
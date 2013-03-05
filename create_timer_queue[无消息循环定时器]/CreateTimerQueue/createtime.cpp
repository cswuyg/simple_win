// createtime.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
/**
不需要使用消息循环的定时器，
譬如：在写公共库的时候很有用，我想让函数库里的某个功能在库被加载后的某个时间定时启动，
我不能要求调用导出函数的线程拥有消息循环。

2013.3.3
cswuyg@gmail.com
*/

VOID WINAPI TimerQueueCallBack(LPVOID pVoid, BOOLEAN bTimeOrWait);

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hTimeQueue = ::CreateTimerQueue();
	HANDLE hTimer = NULL;
	::CreateTimerQueueTimer(&hTimer, hTimeQueue, TimerQueueCallBack, NULL, 1000, 1000, WT_EXECUTEDEFAULT);
	Sleep(5000);
	DeleteTimerQueueEx(hTimeQueue, INVALID_HANDLE_VALUE);
	::wprintf(L"Game over \n");
	getchar();
	return 0;
}

VOID WINAPI TimerQueueCallBack(LPVOID param, BOOLEAN bTimeOrWait)
{
	::wprintf(L"%s \n", L"Hello");
	return;
}
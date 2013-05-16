#include <windows.h>
#include <MMSystem.h>
#pragma comment(lib, "winmm")

/**
http://msdn.microsoft.com/en-us/library/windows/desktop/ff728861(v=vs.85).aspx
Note  This function is obsolete. 
New applications should use CreateTimerQueueTimer to create a timer-queue timer.
timeSetEvent并不被推荐使用

2013.5.10
*/

#include <iostream>

class CTest
{
public:
	void OutPutTest()
	{
		std::cout << "CallBack" << std::endl;
	}
};

MMRESULT g_rSetTimeEvent = 0;

void CALLBACK HighTimerCallBack(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	if(g_rSetTimeEvent == uTimerID)
	{
		static int i = 0;
		CTest* pOut = (CTest*)dwUser;
		pOut->OutPutTest();
		++i;
		if (i == 5)
		{
			timeKillEvent(g_rSetTimeEvent);
		}
	}
}

int main()
{
	TIMECAPS timecaps;
	RtlZeroMemory(&timecaps, sizeof(TIMECAPS));
	if(::timeGetDevCaps(&timecaps, sizeof(TIMECAPS)) != MMSYSERR_NOERROR)
	{
		return -1;
	}
	CTest output_obj;
	g_rSetTimeEvent = ::timeSetEvent(100, 0, HighTimerCallBack, (DWORD)(LONG_PTR)&output_obj, TIME_PERIODIC);
	::Sleep(100000);
	return 0;
}
/**
How to use TimerQueue.
cswuyg@gmail.com
2013.6.8
*/
#include "TimerQueue/timer_queue.h"

#include <windows.h>
#include <iostream>

class TestTimerCallBack_1 : public ITQTimerCallBack
{
public:
	virtual void TimerCallBack()
	{
		std::wcout << L"TestTimerCallback_1" << std::endl;
	}
};


class TestTimerCallBack_2 : public ITQTimerCallBack
{
public:
	virtual void TimerCallBack()
	{
		std::wcout << L"TestTimerCallback_2" << std::endl;
	}
};

void MultiTest()
{
	CTQTimer tqTimer;

	TestTimerCallBack_1 tqCallback_1;
	tqTimer.SetTimer(&tqCallback_1, 1, 1000, FALSE);

	TestTimerCallBack_2 tqCallback_2;
	tqTimer.SetTimer(&tqCallback_2, 2, 1000, TRUE);

	Sleep(6000);
	tqTimer.KillTimer(1);
	tqTimer.KillTimer(2);
	tqTimer.SetTimer(&tqCallback_2, 2, 1000, FALSE);
	Sleep(6000);
	tqTimer.KillTimer(2);
}

int main(int argc, char* argv[])
{
	MultiTest();
	system("pause");
	return 0;
}
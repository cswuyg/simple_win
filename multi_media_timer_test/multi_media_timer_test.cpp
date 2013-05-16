// multi_media_timer_test.cpp : Defines the entry point for the console application.
//
/**
多媒体定时器
cswuyg@gmail.com
2013.5.16
*/
#include "stdafx.h"
#include "MMTimer.h"

#include <iostream>

class CTimerCallback : public ITimerCallBack
{
public:
	virtual void TimerCallBack()
	{
		std::wcout << L"Hello World" << std::endl;
	}

};

void test_func()
{
	CMMTimer timer;
	CTimerCallback callback;
	timer.SetTimer(&callback, 1, 1000, true);
	Sleep(1500);
	timer.KillTimer(1);
}


int _tmain(int argc, _TCHAR* argv[])
{
	test_func();
	system("pause");
	return 0;
}


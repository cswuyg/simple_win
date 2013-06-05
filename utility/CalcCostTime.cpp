#include "stdafx.h"
#include "CalcCostTime.h"



void CCostTime::Start()
{
	::QueryPerformanceCounter(&time_start_);
}


double CCostTime::CostTime()
{
	_LARGE_INTEGER time_end_;	//< 结束时间
	time_end_.HighPart = 0;
	time_end_.LowPart = 0;
	::QueryPerformanceCounter(&time_end_); 
	double dqFreq;				//计时器频率
	LARGE_INTEGER f;		   
	::QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	double dCostTime = (time_end_.QuadPart - time_start_.QuadPart) / dqFreq;
#ifdef _DEBUG
	wchar_t buff[ 1024 ];
	::swprintf_s(buff, L"%s : %0.10f s",  cost_name_.c_str(), dCostTime);
	debug_log(buff);
#endif
	return dCostTime;
}

#ifdef _DEBUG
void CCostTime::debug_log( const std::wstring log_msg )
{
	HWND hSend = ::FindWindow(NULL, (LPCWSTR)&L"cswuyg_debug_Log_wnd");

	COPYDATASTRUCT	copydate;
	copydate.cbData = (DWORD)(log_msg.length() + 1) * sizeof(TCHAR);
	copydate.lpData = (PVOID)log_msg.c_str();

	::SendMessage(hSend, WM_COPYDATA, 0, (LPARAM)&copydate);
}
#endif

CCostTime::CCostTime()
{
	Start();
}



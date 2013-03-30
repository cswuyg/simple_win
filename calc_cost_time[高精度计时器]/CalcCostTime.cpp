
#include "CalcCostTime.h"


void CCostTime::Start()
{
	::QueryPerformanceCounter(&time_start_);
}

void CCostTime::End()
{
	::QueryPerformanceCounter(&time_end_); 
}

double CCostTime::CostTime()
{
	double dqFreq;				//¼ÆÊ±Æ÷ÆµÂÊ
	LARGE_INTEGER f;		   
	::QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	double dCostTime = (time_end_.QuadPart - time_start_.QuadPart) / dqFreq;

	wchar_t buff[ 1024 ];
	::swprintf_s(buff, L"%s : %0.10f s",  cost_name_.c_str(), dCostTime);
	debug_log(buff);

   return dCostTime;
}

void CCostTime::debug_log( const std::wstring log_msg )
{
	::OutputDebugString(log_msg.c_str());
	HWND hSend = ::FindWindow(NULL, (LPCWSTR)&L"cswuyg_debug_Log_wnd");

	COPYDATASTRUCT	copydate;
	copydate.cbData = (DWORD)(log_msg.length() + 1) * sizeof(TCHAR);
	copydate.lpData = (PVOID)log_msg.c_str();

	::SendMessage(hSend, WM_COPYDATA, 0, (LPARAM)&copydate);
}

CCostTime::CCostTime( const wchar_t* log_name )
: cost_name_(log_name)
{
	Start();
}

CCostTime::~CCostTime()
{
	End();
	CostTime();
}
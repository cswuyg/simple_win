#pragma once

#include <windows.h>
#include <string>


/**
用于获取耗时的类
使用QueryPerformanceCounter函数。
*/
class CCostTime
{
public:
	CCostTime();

	void Start();
	double CostTime();
#ifdef _DEBUG
	void debug_log(const std::wstring log_msg);
#endif

private:
	_LARGE_INTEGER time_start_;	//< 开始时间
	std::wstring cost_name_;
	double dCostTime;        //< 耗时 second
};



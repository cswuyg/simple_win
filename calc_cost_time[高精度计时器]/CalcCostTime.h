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
	CCostTime (const wchar_t* log_name);
	~CCostTime();
	void debug_log(const std::wstring log_msg);
private:
	void Start();
	void End();
	double CostTime();
private:
	_LARGE_INTEGER time_start_;	//开始时间
	_LARGE_INTEGER time_end_;	//结束时间
	std::wstring cost_name_;
};




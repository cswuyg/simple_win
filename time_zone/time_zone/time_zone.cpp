/**
test GetTimeZoneInfomation,use Bias to identify the Time Zone.
获取本机所设置的时区
cswuyg@gmail.com
2014.2.14
*/

#include "stdafx.h"
#include <windows.h>

int test_time_zone()
{
	TIME_ZONE_INFORMATION time_zone_info = { 0 };
	::GetTimeZoneInformation(&time_zone_info);
	LONG time_zone = -time_zone_info.Bias;  //UTC = local time + bias
	if ((time_zone >= -12 * 60) && (time_zone < 0))
	{
		//America
		return 1;
	}
	else if ((time_zone >= 0) && (time_zone < 6 * 60))
	{
		//England
		return 2;
	}
	else if ((time_zone >= 6 * 60) && (time_zone < 13 * 60))
	{
		//China
		return 3;
	}
	return 3;
}


int _tmain(int argc, _TCHAR* argv[])
{
	test_time_zone();
	return 0;
}


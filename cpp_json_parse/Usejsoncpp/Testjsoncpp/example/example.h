#pragma once
#include <string>
#include <json/json.h>
#include "../utility/utility.h"

/**
jsoncpp的使用测试
如果不知道json串的数据格式，那么需要使用递归分析。

jsoncpp主要优点是，快，比现有的其它库都快。
jsoncpp主要缺点是不支持unicode，
还有，相比SimpleJson，导出接口多，感觉比较复杂。
工作中，建议使用jsoncpp。
simplejson有一堆坑，用起来很辛苦。
boost property_tree要带上boost库，太庞大。

2012-12-12
*/
class CTestjsoncpp
{
public:
	bool test_Parse();
	bool test_writefile();
	bool test_readfile();

private:
	std::wstring ReleaseJsonStr();
	Json::Value ReleaseJsonValue();
};
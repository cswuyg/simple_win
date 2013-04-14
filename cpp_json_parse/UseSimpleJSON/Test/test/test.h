#pragma  once
#include <JSON.h>
#include <JSONValue.h>
#include <iostream>
class CTestJSON
{
public:
	CTestJSON(){};
	~CTestJSON(){};

	std::wstring GetJsonStr();

	void parseJsonStr(const std::wstring& strJsonStr);
	std::wstring AddJsonStr(const std::wstring& strJsonStr);

private:


};
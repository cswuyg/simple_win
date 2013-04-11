#pragma once
/*
2012-12-10 始
cswuyg@gmail.com
*/
#include <string>
#include <windows.h>
#define IF_BREAK(x) if (x) break;


namespace utility
{
	BOOL WriteToDiskA(const std::wstring& strFilePath, std::string& data);
	BOOL ReadFromDiskA(const std::wstring& strFilePath, std::string& data);
	
	std::wstring int2wstr(int nData);
	std::string wstr2str(const std::wstring& strInput, int nEncode);
	std::wstring str2wstr(const std::string& strInput, int nEncode);
	
	std::wstring GetAppFullName();
	std::wstring GetModuleFullName();
	
	std::wstring GetEnvironmentVariable(const std::wstring& key_name);  //获取环境变量
	void AddEnvironmentVariable(const std::wstring& key_name, const std::wstring& add_value); //添加环境变量某个key的value


	enum ePageCode
	{
		e_ANSI,         //ANSI编码
		e_UNICODE,      //UNICODE
		e_UTF8,         //UTF8
		e_UTF8_NOBOM,   //UTF8 无标志
		e_BigEnd        //大端 无标志 
	};
	bool IsNoBomUTF8Data(char* content, int length);
	ePageCode GetPageCode(char* content, int length);
	
}

#pragma once
/*
2012-12-10
*/
#include <string>
#include <windows.h>
#define IF_BREAK(x) if (x) break;


BOOL WriteToDiskA(const std::wstring& strFilePath, std::string& data);
BOOL ReadFromDiskA(const std::wstring& strFilePath, std::string& data);
std::wstring int2wstr(int nData);
std::string wstr2str(const std::wstring& strInput, int nEncode);
std::wstring str2wstr(const std::string& strInput, int nEncode);

/**
log

how to use: include these two files, and call LogInfo(L"xxxx"), log infomation will be written at %temp% folder;

cswuyg@gmail.com
2013.4
*/

#pragma once
#include "Lock.h"

#include <windows.h>
#include <string>
#include "utility.h"

namespace utility
{
namespace WYGTickLog
{
	extern CCritical g_csLogInstance;

	class CWriteMappingFile
	{
	public:
		CWriteMappingFile();
		~CWriteMappingFile();
		void SetFilePath(const std::wstring& strPath);
		BOOL Write(const std::string& astrInfo);
		BOOL IsValid() const;
	private:
		void RequireFileMapView(LONGLONG llRequireSize);
		void Destroy();
		void CloseMapView();

	private:
		HANDLE m_hMap;
		_LARGE_INTEGER m_llPos;
		_LARGE_INTEGER m_llTotal;
		std::wstring m_strPath;
	};

	class CTickCountLog
	{
	public:
		static CTickCountLog* Instance();
		static void Destroy();
		void writeLogToDisk(LPCWSTR fmt, ...);
		void debugLog(LPCWSTR fmt, ...);
	private:
		CTickCountLog();
		~CTickCountLog();
		std::wstring GetLocalTimeStr();
		std::wstring GetLocalTimeGapStr();
		void simple_log(const std::wstring& log_msg);

	private:
		static CTickCountLog* m_pInstance;
	private:
		CWriteMappingFile m_mappingFile;
		CCritical m_csWriteFile;
	};

#define LogInfo(fmt, ...) CTickCountLog::Instance()->writeLogToDisk(fmt, __VA_ARGS__);
#define LogDebug(fmt, ...) CTickCountLog::Instance()->debugLog(fmt, __VA_ARGS__);
}
}
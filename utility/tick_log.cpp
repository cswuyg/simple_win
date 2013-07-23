#include "stdafx.h"
#include "tick_log.h"
#include <shlwapi.h>

namespace utility
{
namespace WYGTickLog
{
	const int kBlock = 1024 * 1024;

#define ProcessEndLog() CTickCountLog::Instance()->Destroy();

	class CLogInstanceInit
	{
	public:
		CLogInstanceInit()
		{
			LogInfo(L"Start");
		}
		~CLogInstanceInit()
		{
			ProcessEndLog();
		}
	};

	CTickCountLog* CTickCountLog::m_pInstance = NULL;
	CCritical g_csLogInstance;
	CLogInstanceInit g_logInstanceInit;

	CWriteMappingFile::CWriteMappingFile( )
		: m_hMap(NULL)
	{
		m_llPos.QuadPart = 0;
		m_llTotal.QuadPart = 0;
	}

	BOOL CWriteMappingFile::IsValid() const
	{
		return !m_strPath.empty();
	}

	CWriteMappingFile::~CWriteMappingFile()
	{
		Destroy();
	}

	void CWriteMappingFile::CloseMapView()
	{
		if (m_hMap != NULL)
		{
			::CloseHandle(m_hMap);
			m_hMap = NULL;
		}
	}

	void CWriteMappingFile::Destroy()
	{
		CloseMapView();
		HANDLE hFile = ::CreateFile(m_strPath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			::SetFilePointer(hFile, m_llPos.LowPart, &m_llPos.HighPart, FILE_BEGIN);
			::SetEndOfFile(hFile);
			::CloseHandle(hFile);
		}
	}


	void CWriteMappingFile::RequireFileMapView(LONGLONG llRequireSize)
	{
		CloseMapView();
		HANDLE hFile = ::CreateFile(m_strPath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return;
		}
		m_llTotal.QuadPart += llRequireSize;
		_LARGE_INTEGER  llAlign;
		llAlign.QuadPart = m_llTotal.QuadPart % kBlock;
		if (llAlign.QuadPart > 0)
		{
			m_llTotal.QuadPart += kBlock - llAlign.QuadPart;
		}
		m_hMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, m_llTotal.HighPart, m_llTotal.LowPart, NULL);
		::CloseHandle(hFile);
	}

	BOOL CWriteMappingFile::Write(const std::string& astrInfo)
	{
		if(m_llTotal.QuadPart < (m_llPos.QuadPart + astrInfo.length()))
		{
			RequireFileMapView(astrInfo.length());
		}
		unsigned int write_for_align = m_llPos.LowPart % (64 * 1024);
		unsigned int write_high = m_llPos.HighPart;
		unsigned int write_low = m_llPos.LowPart - write_for_align;
		char* pWrite = static_cast<char*>(::MapViewOfFile(m_hMap, FILE_MAP_WRITE, write_high, write_low, astrInfo.length() + write_for_align));
		if (pWrite == NULL)
		{
			return FALSE;
		}
		errno_t eCpyRet = memcpy_s(pWrite + write_for_align, astrInfo.length(), astrInfo.c_str(), astrInfo.length());
		::UnmapViewOfFile(pWrite);
		if (eCpyRet != 0)
		{
			return FALSE;
		}
		m_llPos.QuadPart += astrInfo.length();
		return TRUE;
	}

	void CWriteMappingFile::SetFilePath( const std::wstring& strPath )
	{
		m_strPath = strPath;
	}

	CTickCountLog::CTickCountLog()
	{
		std::wstring strLogTime = GetLocalTimeStr();
		wchar_t szLogPath[MAX_PATH] = {0};
		std::wstring strRightPath = L".\\cswuyg_tick_count_log_";
		strRightPath += strLogTime;
		strRightPath += L".txt";
		std::wstring strPath;
		if (0 < ::GetTempPath(MAX_PATH, szLogPath) && TRUE == ::PathAppend(szLogPath, strRightPath.c_str()))
		{
			strPath = szLogPath;
		}
		m_mappingFile.SetFilePath(strPath);
	}

	void CTickCountLog::writeLogToDisk( LPCWSTR fmt, ... )
	{
		Lock<> lock(m_csWriteFile);
		wchar_t info_buf[4096];
		va_list args;
		va_start(args, fmt);
		vswprintf_s(info_buf, fmt, args);
		va_end(args);

		std::wstring strLogInfo = GetLocalTimeGapStr();
		strLogInfo += L" = ";
		strLogInfo += info_buf;
		if (m_mappingFile.IsValid())
		{
			std::string astrLogInfo = WYGString::wstr2str(strLogInfo, CP_ACP);
			astrLogInfo += "\r\n";
			m_mappingFile.Write(astrLogInfo);
		}

		simple_log(strLogInfo);
	}

	void CTickCountLog::simple_log( const std::wstring& log_msg )
	{
		std::wstring strLogWnd = L"cswuyg_simple_debug_log";
		HWND hSend = ::FindWindow(NULL, strLogWnd.c_str());
		COPYDATASTRUCT copydate;
		copydate.cbData = (DWORD)(log_msg.length() + 1) * sizeof(wchar_t);
		copydate.lpData = (PVOID)log_msg.c_str();
		::SendMessage(hSend, WM_COPYDATA, 0, (LPARAM)&copydate);
	}

	void CTickCountLog::debugLog( LPCWSTR fmt, ... )
	{
		wchar_t info_buf[4096];
		va_list args;
		va_start(args, fmt);
		vswprintf_s(info_buf, fmt, args);
		va_end(args);

		std::wstring strLogInfo = GetLocalTimeGapStr();
		strLogInfo += info_buf;
		simple_log(strLogInfo);
	}

	CTickCountLog::~CTickCountLog()
	{
	}

	std::wstring CTickCountLog::GetLocalTimeStr()
	{
		SYSTEMTIME stTime;
		::memset(&stTime, 0, sizeof(SYSTEMTIME));
		::GetLocalTime(&stTime);
		wchar_t time_buf[64];
		::swprintf_s(time_buf, _countof(time_buf), L"%d%02d%02d%02d%02d%02d", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
		std::wstring strTime = time_buf;
		return strTime;
	}

	std::wstring CTickCountLog::GetLocalTimeGapStr()
	{
		SYSTEMTIME stTime;
		::memset(&stTime, 0, sizeof(SYSTEMTIME));
		::GetLocalTime(&stTime);
		wchar_t time_buf[64];
		::swprintf_s(time_buf, _countof(time_buf), L"%d.%02d.%02d-%02d:%02d:%02d", stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, stTime.wMinute, stTime.wSecond);
		std::wstring strTime = time_buf;
		return strTime;
	}

	CTickCountLog* CTickCountLog::Instance()
	{
		if (m_pInstance == NULL)
		{
			Lock<> lock(g_csLogInstance);
			if (m_pInstance == NULL)
			{
				m_pInstance = new CTickCountLog;
			}
		}
		return m_pInstance;
	}

	void CTickCountLog::Destroy()
	{
		Lock<> lock(g_csLogInstance);
		delete m_pInstance;
		m_pInstance = NULL;
	}

}
}
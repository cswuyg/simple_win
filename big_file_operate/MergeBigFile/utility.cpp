#include "utility.h"
#include <new.h>
#include <Shlwapi.h>
#include <ShlObj.h>
#pragma comment(lib, "Shell32.lib")
#pragma comment( lib, "Shlwapi.lib" )
#include <time.h>
#include <assert.h>

namespace utility
{
	namespace WYGFile
	{

LONGLONG GetFileSize( const std::wstring& strFilePath )
{
	HANDLE hFile = ::CreateFile(strFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	unsigned long nHight;
	unsigned long nLow = ::GetFileSize(hFile, &nHight);
	if (nLow == INVALID_FILE_SIZE)
	{
		DWORD dwError = ::GetLastError();
		if (dwError != NO_ERROR)
		{
			return 0;
		}
	}

	LONGLONG llRet = nHight;
	llRet = llRet << 32;
	llRet += nLow;
	::CloseHandle(hFile);
	return llRet;
}


CMemoryMappedFile::CMemoryMappedFile( const std::wstring& strPath, DWORD CreateFile_dwDesiredAccess, DWORD dwCreationDisposition, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, DWORD MapView_dwDesiredAccess ) 
: m_data(NULL)
, m_llFileSize(0)
, m_hMap(NULL)
, m_dwDesiredAccess(MapView_dwDesiredAccess)
{
	m_llFileSize = GetFileSize(strPath);

	HANDLE hFile = ::CreateFile(strPath.c_str(), CreateFile_dwDesiredAccess, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	m_hMap = ::CreateFileMapping(hFile, NULL, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, NULL);
	::CloseHandle(hFile);
}

bool CMemoryMappedFile::IsValid() const
{
	return m_hMap != NULL;
}

LONGLONG CMemoryMappedFile::length() const
{
	return m_llFileSize;
}

char* CMemoryMappedFile::MapView(DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap )
{
	if (m_data != NULL)
	{
		::UnmapViewOfFile(m_data);
	}
	m_data = static_cast<char*>(::MapViewOfFile(m_hMap, m_dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap));
	return m_data;
}

CMemoryMappedFile::~CMemoryMappedFile()
{
	if (m_data != NULL)
	{
		::UnmapViewOfFile(m_data);
	}
	if (m_hMap != NULL)
	{
		::CloseHandle(m_hMap);
	}
}


	} //WYGFile


}
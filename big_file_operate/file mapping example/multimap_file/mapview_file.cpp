#include "mapview_file.h"



CMultiMappedFile::CMultiMappedFile( const std::wstring& strPath, DWORD CreateFile_dwDesiredAccess, DWORD dwCreationDisposition, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, DWORD MapView_dwDesiredAccess, const std::wstring& strMappingName )
: m_hFile(INVALID_HANDLE_VALUE)
, m_hMap(NULL)
, m_dwDesiredAccess(MapView_dwDesiredAccess)
, m_pData(NULL)
{
	m_hFile = ::CreateFile(strPath.c_str(), CreateFile_dwDesiredAccess, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	if (strMappingName.empty())
	{
		m_hMap = ::CreateFileMapping(m_hFile, NULL, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, NULL);
	}
	else
	{
		m_hMap = ::CreateFileMapping(m_hFile, NULL, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, strMappingName.c_str());
	}
}

CMultiMappedFile::CMultiMappedFile( const std::wstring& strMappingName, DWORD MapView_dwDesiredAccess )
: m_hFile(INVALID_HANDLE_VALUE)
, m_hMap(NULL)
, m_dwDesiredAccess(MapView_dwDesiredAccess)
, m_pData(NULL)
{
	if (strMappingName.empty())
	{
		return;
	}
	m_hMap = ::OpenFileMapping(m_dwDesiredAccess, NULL, strMappingName.c_str());
}

BOOL CMultiMappedFile::IsValid() const
{
	return m_hMap != NULL;
}

char* CMultiMappedFile::MapView(DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap )
{
	m_pData = static_cast<char*>(::MapViewOfFile(m_hMap, m_dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap));
	return m_pData;
}

CMultiMappedFile::~CMultiMappedFile()
{
	Destroy();
}

void CMultiMappedFile::Destroy()
{
	UnMapView();
	if (m_hMap != NULL)
	{
		::CloseHandle(m_hMap);
		m_hMap = NULL;
	}
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

void CMultiMappedFile::UnMapView()
{
	if (m_pData != NULL)
	{
		::UnmapViewOfFile(m_pData);
		m_pData = NULL;
	}
}

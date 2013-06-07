#pragma once
#include <iostream>
#include <windows.h>

class CMultiMappedFile
{
public:
	CMultiMappedFile(const std::wstring& strPath, DWORD CreateFile_dwDesiredAccess, DWORD dwCreationDisposition, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, DWORD MapView_dwDesiredAccess, const std::wstring& strMappingName);
	CMultiMappedFile(const std::wstring& strMappingName, DWORD MapView_dwDesiredAccess);
	~CMultiMappedFile();

	char* MapView(DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap);
	void UnMapView();

	BOOL IsValid() const;

	void Destroy();

private:
	HANDLE m_hFile;
	HANDLE m_hMap;
	DWORD m_dwDesiredAccess;
	char* m_pData;
};
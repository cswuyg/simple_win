#include "utility.h"

BOOL WriteToDiskA( const std::wstring& strFilePath, std::string& data )
{
	BOOL bRet = FALSE;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	do 
	{
		std::wstring strFilePathTmp = strFilePath + L".tmp";
		hFile = ::CreateFile(strFilePathTmp.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		IF_BREAK(hFile == INVALID_HANDLE_VALUE);
		DWORD dwWrite = 0;
		DWORD dwLength = (DWORD)data.length();
		::WriteFile(hFile, data.c_str(), dwLength, &dwWrite, NULL);
		IF_BREAK(dwWrite != dwLength);
		::CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
		bRet = ::MoveFileEx(strFilePathTmp.c_str(), strFilePath.c_str(), MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH);
	} while (FALSE);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
	}
	return bRet;
}

BOOL ReadFromDiskA( const std::wstring& strFilePath, std::string& data )
{
	BOOL bRet = FALSE;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	do 
	{
		hFile = ::CreateFile(strFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		IF_BREAK(hFile == INVALID_HANDLE_VALUE);

		DWORD nCount = ::GetFileSize(hFile, NULL);
		if (nCount == INVALID_FILE_SIZE)
		{
			//DWORD dwErr = GetLastError();
			break;
		}
		DWORD nReturnCount = 0;
		char* buf = new char[nCount+1];
		::memset(buf, 0, nCount + 1);
		::ReadFile(hFile, buf, nCount, &nReturnCount, NULL);
		if (nReturnCount != nCount)
		{
			delete [] buf;
			break;
		}
		data = std::string(buf, nCount);
		delete [] buf;

		::CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
		bRet = TRUE;
	} while (FALSE);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
	}
	return bRet;
}

std::wstring int2wstr( int nData )
{
	wchar_t wchBuf[32] = L"";
	::swprintf_s(wchBuf, L"%d", nData);
	return wchBuf;
}

std::string wstr2str( const std::wstring& strInput, int nEncode )
{
	if (strInput.empty())
	{
		return "";
	}
	int nLength = ::WideCharToMultiByte(nEncode, 0, strInput.c_str(), -1, 0, 0, 0, 0);
	char* buf = new char[nLength + 1];
	::memset(buf, 0, nLength + 1);
	::WideCharToMultiByte(nEncode, 0, strInput.c_str(), -1, buf, nLength, 0, 0);
	std::string strResult = buf;
	delete[] buf;
	return strResult;
}

std::wstring str2wstr( const std::string& strInput, int nEncode )
{
	if( strInput.empty() )
	{
		return L"";
	}
	int nLength = ::MultiByteToWideChar( nEncode, 0, strInput.c_str(), -1, 0, 0 );
	wchar_t* buf = new wchar_t[nLength + 1];
	::memset(buf, 0, ( nLength + 1 ) * 2);
	::MultiByteToWideChar(nEncode, 0, strInput.c_str(), -1, buf, nLength);
	std::wstring strResult = buf;
	delete[] buf;
	return strResult;
}
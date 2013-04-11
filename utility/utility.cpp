#include "utility.h"

namespace utility
{
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
		data = buf;
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

std::wstring GetAppFullName()
{
	static std::wstring aplication_fullname;
	if(aplication_fullname.empty())
	{
		wchar_t lpszFileName[MAX_PATH] = {0};
		::GetModuleFileName(NULL, lpszFileName, MAX_PATH);
		aplication_fullname = lpszFileName;
	}
	return aplication_fullname;
}


std::wstring GetModuleFullName()
{
	static std::wstring module_fullname;
	if(module_fullname.empty())
	{
		static int lpAddress = 0;
		HMODULE hMod = 0;
		::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)&lpAddress, &hMod);
		wchar_t lpszName[MAX_PATH] = {0};
		::GetModuleFileName(hMod, lpszName, MAX_PATH);
		module_fullname = lpszName;
	}
	return module_fullname;
}

std::wstring GetEnvironmentVariable( const std::wstring& key_name )
{
	DWORD require_size = ::GetEnvironmentVariable(key_name.c_str(), NULL, 0);
	wchar_t* buf = new wchar_t[require_size];
	::GetEnvironmentVariable(key_name.c_str(), buf, require_size);
	return buf;

	//下边的实现方式也可以
	//std::wstring new_key = L"%";
	//new_key += key_name;
	//new_key += L"%";
	//DWORD chValue = ::ExpandEnvironmentStrings(new_key.c_str(), NULL, 0);
	//wchar_t* buf = new wchar_t[chValue];
	//::ExpandEnvironmentStrings(new_key.c_str(), buf, chValue);
	//return buf;
}

void AddEnvironmentVariable( const std::wstring& key_name, const std::wstring& add_value )
{
	std::wstring new_value = GetEnvironmentVariable(key_name);
	new_value += L";";
	new_value += add_value;
	::SetEnvironmentVariable(key_name.c_str(), new_value.c_str());
}


utility::ePageCode GetPageCode( char* content, int length )
{
	ePageCode ret = e_ANSI;
	if (length >= 2)
	{
		byte b0 = *content;
		byte b1 = *(content+1);
		byte b2 = (length >= 3 ? *(content+2) : 0);
		if (b0 == 0xFF && b1 == 0xFE)
		{
			ret = e_UNICODE;
		}
		else if (b0 == 0xFE && b1 == 0xFF)
		{
			ret = e_BigEnd;
		}
		else if (b0 == 0xEF && b1 == 0xBB && b2 == 0xBF)
		{
			ret = e_UTF8;
		}
		else if (IsNoBomUTF8Data(content, length))
		{
			ret = e_UTF8_NOBOM;
		}
	}
	return ret;
}

bool IsNoBomUTF8Data( char* content, int length )
{
	int i = 0;
	int size = length;

	while(i < size)
	{
		int step = 0;
		if((content[i] & 0x80) == 0x00)
		{
			step = 1;
		}
		else if((content[i] & 0xe0) == 0xc0)
		{
			if(i + 1 >= size) return false;
			if((content[i + 1] & 0xc0) != 0x80)
			{
				return false;
			}
			step = 2;
		}
		else if((content[i] & 0xf0) == 0xe0)
		{
			if(i + 2 >= size)
			{
				return false;
			}
			if((content[i + 1] & 0xc0) != 0x80) 
			{
				return false;
			}
			if((content[i + 2] & 0xc0) != 0x80)
			{
				return false;
			}
			step = 3;
		}
		else
		{
			return false;
		}

		i += step;
	}

	if(i == size) 
	{
		return true;
	}
	return false;
}

}
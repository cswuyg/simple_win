/**
测试文件/文件夹的合法字符
2013.6.17
cswuyg
*/
#include <windows.h>
#include <string>
#include <Shlwapi.h>
#include <ShlObj.h>

#pragma comment(lib, "Shlwapi.lib")

/**
@brief 检查strPath路径下的字符是否是有效字符
@note strPath 必须是包含盘符的完整路径
*/
BOOL IsLegalPathCharacter(const std::wstring& strPath)
{
	if (strPath.length() < 4)
	{
		return FALSE;
	}
	BOOL bRet = TRUE;
	std::wstring strExclude = L"\\/:*?\"<>|";      //illegal characters
	wchar_t* pBufName = new wchar_t[strPath.length() + 1];
	::memset(pBufName, 0, strPath.length()+1);
	::wcsncpy_s(pBufName, strPath.length()+1, strPath.c_str(), strPath.length());
	std::wstring strDisk = strPath.substr(0, 3);
	if (strDisk[1] != L':' || (strDisk[2] != L'\\' && strDisk[2] != L'/'))
	{
		return FALSE;
	}
	BOOL bDiskExist = ::PathFileExists(strDisk.c_str());
	if (!bDiskExist)
	{
		return FALSE;
	}
	wchar_t* pNoRootName = pBufName+3;//::PathSkipRoot(pBufName);
	::PathStripPath(pNoRootName);
	std::wstring strName = pNoRootName;
	::wcsncpy_s(pBufName, strParentPath.length()+1, strParentPath.c_str(), strParentPath.length());
	::PathRemoveFileSpec(pBufName);
	strParentPath = pBufName;
	while(!strName.empty())
	{
		std::wstring::size_type pos = strName.find_first_of(strExclude);
		if (pos == std::wstring::npos)
		{
			::wcsncpy_s(pBufName, strParentPath.length()+1, strParentPath.c_str(), strParentPath.length());
			::PathStripPath(pBufName);
			strName = pBufName;
			::wcsncpy_s(pBufName, strParentPath.length()+1, strParentPath.c_str(), strParentPath.length());
			::PathRemoveFileSpec(pBufName);
			strParentPath = pBufName;
		}
		else
		{
			bRet = FALSE; 
			break;
		}
	} 
	delete [] pBufName;
	return bRet;
}

/**
@brierf 获取合法的名称
@note strName 必须只能是name不能包含有
*/
std::wstring GetLegalName(const std::wstring& strName)
{
	wchar_t* pBuf = new wchar_t[strName.length() + 1];
	::memset(pBuf, 0, strName.length() + 1);
	::wcsncpy_s(pBuf, strName.length()+1, strName.c_str(), strName.length());
	int nRet = ::PathCleanupSpec(NULL, pBuf);
	if (nRet != PCS_PATHTOOLONG && nRet != PCS_FATAL)
	{
		return pBuf;
	}
	return L"";
}


int main(int argc, wchar_t* argv[])
{
	BOOL bLegal = IsLegalPathCharacter(L"c?:\\cswuyg");
	bLegal = IsLegalPathCharacter(L"c:\\cswuyg?");
	bLegal = IsLegalPathCharacter(L"c:\\cswuyg");
	std::wstring strLegalName = GetLegalName(L"/cswuyg\\/:*?\"<>|");   //"/"导致多了一个"-"
	::system("pause");
	return 0;
}


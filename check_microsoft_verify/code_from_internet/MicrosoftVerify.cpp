#include <stdafx.h>
#include <stdio.h>
#include <stdlib.h>
#include "MicrosoftVerify.h"
//filename:MicrosoftVerify.c

//代码参考自网络，来源：http://yulv.net/archives/60


enum E_TrustFileResust
{
	e_none,
	e_suc,
	e_fail,
};

E_TrustFileResust CheckFileTrust(const std::wstring file_name)
{
	HCATADMIN hCatAdmin = NULL;
	if ( !CryptCATAdminAcquireContext(&hCatAdmin, NULL, 0))
	{
		return e_none;
	}
	HANDLE hFile = ::CreateFile(file_name.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CryptCATAdminReleaseContext(hCatAdmin, 0);
		return e_none;
	}
	DWORD dwCnt = 0;
	BOOL bHash = CryptCATAdminCalcHashFromFileHandle(hFile, &dwCnt, NULL, 0);
	if (!bHash)
	{
		::CloseHandle(hFile);
		CryptCATAdminReleaseContext(hCatAdmin, 0);
		return e_none;
	}
	BYTE* bit_hash = new BYTE[dwCnt];
	::memset(bit_hash, 0, dwCnt);
	bHash = CryptCATAdminCalcHashFromFileHandle(hFile, &dwCnt, bit_hash, 0);
	::CloseHandle(hFile);
	if (!bHash)
	{
		delete [] bit_hash;
		CryptCATAdminReleaseContext(hCatAdmin, 0);
		return e_none;
	}
	WINTRUST_DATA wintrust_data = { 0 };
	WINTRUST_FILE_INFO wintrust_file_info = { 0 };
	WINTRUST_CATALOG_INFO wintrust_catalog_info = { 0 };
	CATALOG_INFO catalog_info = { 0 };
	wchar_t* pszMemberTag = NULL;
	HCATINFO hCatInfo = CryptCATAdminEnumCatalogFromHash(hCatAdmin, bit_hash, dwCnt, 0, NULL);
	if (hCatInfo == NULL)
	{
		wintrust_file_info.cbStruct = sizeof(WINTRUST_FILE_INFO);
		wintrust_file_info.pcwszFilePath = file_name.c_str();
		wintrust_file_info.hFile = NULL;
		wintrust_file_info.pgKnownSubject = NULL;
		wintrust_data.cbStruct = sizeof(WINTRUST_DATA);
		wintrust_data.dwUnionChoice = WTD_CHOICE_FILE;
		wintrust_data.pFile = &wintrust_file_info; 
		wintrust_data.dwUIChoice = WTD_UI_NONE;
		wintrust_data.fdwRevocationChecks = WTD_REVOKE_NONE;
		wintrust_data.dwStateAction = WTD_STATEACTION_IGNORE;
		wintrust_data.dwProvFlags = WTD_SAFER_FLAG;
		wintrust_data.hWVTStateData = NULL;
		wintrust_data.pwszURLReference = NULL;
	}
	else
	{
		pszMemberTag = new wchar_t[dwCnt * 2 + 1];
		//将hash的结果 变换成unicode 例如0x8c -> 38 00 43 00
		for (DWORD i = 0; i < dwCnt; ++i)
		{
			wsprintfW( &pszMemberTag[i * 2], L"%02X", bit_hash[i] );
		}
		CryptCATCatalogInfoFromContext( hCatInfo, &catalog_info, 0 );
		wintrust_catalog_info.cbStruct = sizeof( WINTRUST_CATALOG_INFO );
		wintrust_catalog_info.pcwszCatalogFilePath = catalog_info.wszCatalogFile;
		wintrust_catalog_info.pcwszMemberFilePath = file_name.c_str();
		wintrust_catalog_info.pcwszMemberTag = pszMemberTag;
		wintrust_data.cbStruct = sizeof( WINTRUST_DATA );
		wintrust_data.dwUnionChoice = WTD_CHOICE_CATALOG;
		wintrust_data.pCatalog = &wintrust_catalog_info;
		wintrust_data.dwUIChoice = WTD_UI_NONE;
		wintrust_data.fdwRevocationChecks = WTD_STATEACTION_VERIFY;
		wintrust_data.dwProvFlags = 0;
		wintrust_data.hWVTStateData = NULL;
		wintrust_data.pwszURLReference = NULL;
	}

	GUID action = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	HRESULT hr = WinVerifyTrustX(NULL, &action, &wintrust_data);
	BOOL bRet = SUCCEEDED(hr);
	if (hCatInfo != NULL)
	{
		CryptCATAdminReleaseCatalogContext(hCatAdmin, hCatInfo, 0);
	}
	CryptCATAdminReleaseContext(hCatAdmin, 0); 
	if (pszMemberTag != NULL)
	{
		delete [] pszMemberTag;
	}
	if (bit_hash != NULL)
	{
		delete [] bit_hash;
	}
	if (bRet)
	{
		return e_suc;
	}
	else
	{
		return e_fail;
	}
}
BOOL InitFunc()
{
	HMODULE dll_handle = ::LoadLibrary(L"Wintrust.dll");
	if (dll_handle == NULL)
	{
		return FALSE;
	}
	(FARPROC &)CryptCATAdminAcquireContext = ::GetProcAddress(dll_handle,"CryptCATAdminAcquireContext");
	(FARPROC &)CryptCATAdminReleaseContext = ::GetProcAddress(dll_handle,"CryptCATAdminReleaseContext");
	(FARPROC &)CryptCATAdminCalcHashFromFileHandle = ::GetProcAddress(dll_handle,"CryptCATAdminCalcHashFromFileHandle");
	(FARPROC &)CryptCATAdminEnumCatalogFromHash = ::GetProcAddress(dll_handle,"CryptCATAdminEnumCatalogFromHash");
	(FARPROC &)CryptCATAdminReleaseCatalogContext = ::GetProcAddress(dll_handle,"CryptCATAdminReleaseCatalogContext");
	(FARPROC &)CryptCATCatalogInfoFromContext = ::GetProcAddress(dll_handle,"CryptCATCatalogInfoFromContext");
	(FARPROC &)WinVerifyTrustX = ::GetProcAddress(dll_handle,"WinVerifyTrust");
	if (CryptCATAdminAcquireContext == NULL
		|| CryptCATAdminReleaseContext == NULL
		|| CryptCATAdminCalcHashFromFileHandle == NULL
		|| CryptCATAdminEnumCatalogFromHash == NULL
		|| CryptCATAdminReleaseCatalogContext == NULL
		|| CryptCATCatalogInfoFromContext == NULL 
		|| WinVerifyTrustX == NULL) 
	{
		return FALSE;
	}
	return TRUE;
}

const wchar_t* const kPath = L"D:\\work\\pcfaster mini包\\PC_Faster_Setup_Mini 3.7.1.44601.exe";

int main(int argc,char **argv)
{
	if (!InitFunc())
	{
		printf("%s %d error!",__FILE__,__LINE__);
		return 0;
	}
	E_TrustFileResust ret = CheckFileTrust(kPath);
	if (ret == e_fail)
	{
		printf("!/n");
	}
	else
	{
		printf("Sorry!/n");
	}
	return 0;
} 
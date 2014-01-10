#ifndef _MICROSOFTVERIFY_H_
#define _MICROSOFTVERIFY_H_
//filename:MicrosoftVerify.h
//¨°????¨¢11?¡§¨°?¨¤¡ä¡Á?ReactOS
#include "windows.h"
#include <string>
typedef struct WINTRUST_FILE_INFO_
{
    DWORD   cbStruct;
    LPCWSTR pcwszFilePath;
    HANDLE  hFile;
    GUID*   pgKnownSubject;
} WINTRUST_FILE_INFO, *PWINTRUST_FILE_INFO;



typedef void *HCERTSTORE;
typedef void *HCRYPTMSG;

typedef const CTL_CONTEXT *PCCTL_CONTEXT;
typedef struct WINTRUST_CATALOG_INFO_
{
    DWORD         cbStruct;
    DWORD         dwCatalogVersion;
    LPCWSTR       pcwszCatalogFilePath;
    LPCWSTR       pcwszMemberTag;
    LPCWSTR       pcwszMemberFilePath;
    HANDLE        hMemberFile;
    BYTE*         pbCalculatedFileHash;
    DWORD         cbCalculatedFileHash;
PCCTL_CONTEXT pcCatalogContext;
} WINTRUST_CATALOG_INFO, *PWINTRUST_CATALOG_INFO;
typedef struct WINTRUST_BLOB_INFO_
{
    DWORD   cbStruct;
    GUID    gSubject;
    LPCWSTR pcwszDisplayName;
    DWORD   cbMemObject;
    BYTE*   pbMemObject;
    DWORD   cbMemSignedMsg;
    BYTE*   pbMemSignedMsg;
} WINTRUST_BLOB_INFO, *PWINTRUST_BLOB_INFO;


typedef struct WINTRUST_SGNR_INFO_
{
    DWORD             cbStruct;
    LPCWSTR           pcwszDisplayName;
    CMSG_SIGNER_INFO* psSignerInfo;
    DWORD             chStores;
    HCERTSTORE*       pahStores;
} WINTRUST_SGNR_INFO, *PWINTRUST_SGNR_INFO;




typedef struct WINTRUST_CERT_INFO_
{
    DWORD         cbStruct;
    LPCWSTR       pcwszDisplayName;
    CERT_CONTEXT* psCertContext;
    DWORD         chStores;
    HCERTSTORE*   pahStores;
    DWORD         dwFlags;
    FILETIME*     psftVerifyAsOf;
} WINTRUST_CERT_INFO, *PWINTRUST_CERT_INFO;
typedef struct _WINTRUST_DATA
{
    DWORD  cbStruct;
    LPVOID pPolicyCallbackData;
    LPVOID pSIPClientData;
    DWORD  dwUIChoice;
    DWORD  fdwRevocationChecks;
    DWORD  dwUnionChoice;
    union
    {
        struct WINTRUST_FILE_INFO_*    pFile;
        struct WINTRUST_CATALOG_INFO_* pCatalog;
        struct WINTRUST_BLOB_INFO_*    pBlob;
        struct WINTRUST_SGNR_INFO_*    pSgnr;
        struct WINTRUST_CERT_INFO_*    pCert;
    } DUMMYUNIONNAME;
    DWORD  dwStateAction;
    HANDLE hWVTStateData;
    WCHAR* pwszURLReference;
    DWORD  dwProvFlags;
    DWORD  dwUIContext;
} WINTRUST_DATA, *PWINTRUST_DATA;
typedef struct CATALOG_INFO_ 
{
    DWORD cbStruct;
    WCHAR wszCatalogFile[MAX_PATH];
} CATALOG_INFO;
typedef HANDLE HCATADMIN;
typedef HANDLE HCATINFO;
//o¡¥¨ºy?¡§¨°? 
BOOL (WINAPI *CryptCATAdminAcquireContext)(HCATADMIN*,const GUID*,DWORD);
BOOL (WINAPI *CryptCATAdminReleaseContext)(HCATADMIN,DWORD);
BOOL (WINAPI *CryptCATAdminCalcHashFromFileHandle)(HANDLE,DWORD*,BYTE*,DWORD);
HCATINFO  (WINAPI *CryptCATAdminEnumCatalogFromHash)(HCATADMIN,BYTE*,DWORD,DWORD,HCATINFO*);
BOOL (WINAPI *CryptCATAdminReleaseCatalogContext)(HCATADMIN,HCATINFO,DWORD);
BOOL (WINAPI *CryptCATCatalogInfoFromContext)(HCATINFO,CATALOG_INFO*,DWORD);
LONG (WINAPI *WinVerifyTrustX)( HWND hwnd,GUID *ActionID,LPVOID  ActionData);
/* dwUIChoice */
#define WTD_UI_ALL                1
#define WTD_UI_NONE               2
#define WTD_UI_NOBAD              3
#define WTD_UI_NOGOOD             4
/* fdwRevocationChecks */
#define WTD_REVOKE_NONE           0
#define WTD_REVOKE_WHOLECHAIN     1
/* dwUnionChoice */
#define WTD_CHOICE_FILE           1
#define WTD_CHOICE_CATALOG        2
#define WTD_CHOICE_BLOB           3
#define WTD_CHOICE_SIGNER         4
#define WTD_CHOICE_CERT           5
#define WTD_STATEACTION_IGNORE           0
#define WTD_STATEACTION_VERIFY           1
#define WTD_STATEACTION_CLOSE            2
#define WTD_STATEACTION_AUTO_CACHE       3
#define WTD_STATEACTION_AUTO_CACHE_FLUSH 4
#define WTD_SAFER_FLAG                          0x00000100
#define WINTRUST_ACTION_GENERIC_VERIFY_V2 { 0xaac56b,   0xcd44, 0x11d0, { 0x8c,0xc2,0x00,0xc0,0x4f,0xc2,0x95,0xee }}
#endif
#include "stdafx.h"
#include "start_gui.h"


#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "Userenv.lib")

StartGUIProc::StartGUIProc(const std::wstring& processPath, const std::wstring& arguments)
: processPath_(processPath), arguments_(arguments)
{
}

HANDLE StartGUIProc::GetCurrentUserToken()
{
    PWTS_SESSION_INFO pSessionInfo = 0;
    DWORD dwCount = 0;
    ::WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &dwCount);
    int session_id = 0;
    for (DWORD i = 0; i < dwCount; ++i)
    {
        WTS_SESSION_INFO si = pSessionInfo[i];
        if (WTSActive == si.State)
        {
            session_id = si.SessionId;
            break;
        }
    }
    ::WTSFreeMemory(pSessionInfo);
    HANDLE current_token = 0;
    BOOL bRet = ::WTSQueryUserToken(session_id, &current_token);
    int errorcode = GetLastError();
    if (bRet == false)
    {
        return 0;
    }
    HANDLE primaryToken = 0;
    bRet = ::DuplicateTokenEx(current_token, TOKEN_ASSIGN_PRIMARY | TOKEN_ALL_ACCESS, 0, SecurityImpersonation, TokenPrimary, &primaryToken);
    errorcode = GetLastError();
    if (bRet == false)
    {
        return 0;
    }
    return primaryToken;
}

BOOL StartGUIProc::Run()
{
    HANDLE primaryToken = GetCurrentUserToken();
    if (primaryToken == 0)
    {
        return FALSE;
    }
	STARTUPINFO StartupInfo = {0};
    PROCESS_INFORMATION processInfo;
    StartupInfo.cb = sizeof(STARTUPINFO);

    std::wstring command = L"\"" + processPath_ + L"\"";
    if (arguments_.length() != 0)
    {
        command += L" " + arguments_;
    }
    void* lpEnvironment = NULL;
    BOOL resultEnv = ::CreateEnvironmentBlock(&lpEnvironment, primaryToken, FALSE);
    if (resultEnv == 0)
    {                                
        long nError = GetLastError();                                
    }
    BOOL result = ::CreateProcessAsUser(primaryToken, 0, (LPWSTR)(command.c_str()), NULL, NULL, FALSE, CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT, NULL, 0, &StartupInfo, &processInfo);
    ::DestroyEnvironmentBlock(lpEnvironment);
    ::CloseHandle(primaryToken);
    return result;
}
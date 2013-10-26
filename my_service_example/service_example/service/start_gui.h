/**
start GUI process from service application
cswuyg@gmail.com
2013.10.26


´úÂë²Î¿¼£ºhttp://www.codeproject.com/Articles/36581/Interaction-between-services-and-applications-at-u
*/
#pragma once

class StartGUIProc
{
public:
    StartGUIProc(const std::wstring& processPath, const std::wstring& arguments = L"");
    BOOL Run();

private:
    HANDLE GetCurrentUserToken();
    
private:
    std::wstring processPath_;
    std::wstring arguments_;
};

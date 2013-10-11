/**
A very simple example about create a tray icon.
cswuyg@gmail.com
*/
#include "stdafx.h"
#include <windows.h>

#define WM_USER_TRAY 3333

NOTIFYICONDATA m_stNID;

void CreateTrayIcon()
{
	::memset(&m_stNID, 0, sizeof(NOTIFYICONDATA));
	m_stNID.cbSize = (DWORD)sizeof(NOTIFYICONDATA);   
	m_stNID.hWnd = NULL;  
	m_stNID.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP ;
	m_stNID.uCallbackMessage = WM_USER_TRAY;

	wcscpy_s(m_stNID.szTip, 256, L"Test.....");
	m_stNID.hIcon = (HICON)::LoadImage(NULL, L"Player.ico", IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON), 
		LR_DEFAULTCOLOR | LR_LOADFROMFILE); 
	::Shell_NotifyIcon(NIM_ADD, &m_stNID);
}


int _tmain(int argc, _TCHAR* argv[])
{
	CreateTrayIcon();
	Sleep(1000000);
	system( "pause" );
	return 0;
}


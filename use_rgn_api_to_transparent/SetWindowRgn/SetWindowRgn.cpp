// SetWindowRgn.cpp : Defines the entry point for the application.
//

#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "transparent/transparent.h"

#include "SetWindowRgn.h"

#include <string>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
const TCHAR szWindowClass[] = L"cswuyg_test_transparent_wnd";			// the main window class name
HWND hParent;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	// Main message loop:	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	RtlZeroMemory(&wcex, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+2);
	wcex.lpszClassName	= szWindowClass;

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = ::CreateWindowEx(NULL, szWindowClass, NULL, WS_CLIPCHILDREN,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	hParent = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_TIMER:
		{
			HDC hDC=::GetDC(hWnd);
			static int i = 0;
			i += 100;
			RECT client_rc;
			::GetClientRect(hParent, &client_rc);
			HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,i % 256));
			::FillRect(hDC, &client_rc, hBrush);	
			::ReleaseDC(NULL,hDC);
			break;
		}
	case WM_CREATE:
		{
			SetTimer(hWnd, 100, 3000, NULL);
			break;
		}
	case WM_PAINT:
		{
			if (hWnd == hParent)
			{
				hdc = ::BeginPaint(hWnd, &ps);
				//RECT client_rc;
				//::GetClientRect(hParent, &client_rc);
				//HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
				//::FillRect(hdc, &client_rc, hBrush);
				::EndPaint(hWnd, &ps);
			}
			else
			{
				//使用BitmapToRegion实现透明模式
				{
					hdc = BeginPaint(hWnd, &ps);
					CTransparent transparent_model;
					std::wstring text = L"It is the time you have wasted for your rose that makes your rose so important.";
					transparent_model.OnPaint(hWnd, hdc, text);
					EndPaint(hWnd, &ps);
				}
				//普通模式
				//{
				//	hdc = BeginPaint(hWnd, &ps);
				//	::SetBkMode(hdc, TRANSPARENT);
				//	RECT client_rc;
				//	::GetClientRect(hParent, &client_rc);
				//	::SetTextColor(hdc, RGB(255, 255, 40));
				//	::DrawText(hdc, L"It is the time you have wasted for your rose that makes your rose so important.", 
				//		-1, 
				//		&client_rc, 
				//		DT_LEFT | DT_NOCLIP | DT_NOPREFIX);
				//	EndPaint(hWnd, &ps);
				//}
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ERASEBKGND:
		return true;
		break;
	case WM_LBUTTONDOWN:
		{
			static int i = 0;
			if (i == 0)
			{
				++i;
				HWND hWnd = ::CreateWindowEx(NULL, szWindowClass, NULL, WS_CHILD ,
					10, 100, 600, 22, hParent, NULL, hInst, NULL);
				ShowWindow(hWnd, SW_SHOW);
			}

		}
		PostMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

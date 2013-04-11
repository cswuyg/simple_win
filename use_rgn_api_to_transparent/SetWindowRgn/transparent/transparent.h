#pragma once
#include <windows.h>
#include <string>

class CTransparent
{
public:
	void OnPaint(HWND hwnd, HDC hdc, const std::wstring& text);

private:
	 HRGN BitmapToRegion (HBITMAP hDDBBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010);
};
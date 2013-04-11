#include "transparent.h"

//	BitmapToRegion :	Create a region from the "non-transparent" pixels of a bitmap
//	Author :			Jean-Edouard Lachand-Robert (http://www.geocities.com/Paris/LeftBank/1160/resume.htm), June 1998.
//
//	hDDBBmp :				Source bitmap
//	cTransparentColor :	Color base for the "transparent" pixels (default is black)
//	cTolerance :		Color tolerance for the "transparent" pixels.
//
//	A pixel is assumed to be transparent if the value of each of its 3 components (blue, green and red) is 
//	greater or equal to the corresponding value in cTransparentColor and is lower or equal to the 
//	corresponding value in cTransparentColor + cTolerance.

//2011.10.11
//cTransparentColor ~ cTransparentColor+cTolerance之间的颜色透明 /
//这个函数的做法是：把传入的DDB位图换成DIB保存，然后遍历每一个像素点，把需要保留的像素点区域记录下来，最后创建Region，返回该Region的句柄
//修整:
//原来的代码有内存泄漏，只有GlobalAlloc，没有GlobalFree，并且这些内存分配函数是被建议不使用的。换成Heap
//变量名也换了下
HRGN CTransparent::BitmapToRegion( HBITMAP hDDBBmp, COLORREF cTransparentColor /*= 0*/, COLORREF cTolerance /*= 0x101010*/ )
{
	if ( hDDBBmp == NULL )
		return NULL;

	HRGN hRgn = NULL;
	HDC hMemDC = CreateCompatibleDC( NULL );
	if ( hMemDC == NULL )
		return NULL;

	//创建一个DIB位图
	BITMAP bmDDB;
	GetObject( hDDBBmp, sizeof(BITMAP), &bmDDB );
	BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
		sizeof(BITMAPINFOHEADER),	// biSize 
		bmDDB.bmWidth,				// biWidth; 
		bmDDB.bmHeight,			// biHeight; 
		1,							// biPlanes; 
		32,						// biBitCount 
		BI_RGB,					// biCompression; 
		0,							// biSizeImage; 
		0,							// biXPelsPerMeter; 
		0,							// biYPelsPerMeter; 
		0,							// biClrUsed; 
		0							// biClrImportant; 
	};
	VOID * pbits32; 
	HBITMAP hDIBBmp = CreateDIBSection( hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0 );
	if ( hDIBBmp == NULL )
	{
		DeleteDC( hMemDC );
		return NULL;
	}

	HBITMAP holdBmp = ( HBITMAP )SelectObject( hMemDC, hDIBBmp );
	//通过DC把DDB位图拷贝到DIB位图中
	HDC hDC = CreateCompatibleDC( hMemDC );
	holdBmp = (HBITMAP)SelectObject( hDC, hDDBBmp );
	BitBlt( hMemDC, 0, 0, bmDDB.bmWidth, bmDDB.bmHeight, hDC, 0, 0, SRCCOPY );

	SelectObject( hDC, holdBmp );
	DeleteDC( hDC );

	BITMAP bmDIB;
	GetObject( hDIBBmp, sizeof(BITMAP), &bmDIB );
	bmDIB.bmWidth += ( bmDIB.bmWidth & 3 + 3 ) / 4;


	// For better performances, we will use the ExtCreateRegion() function to create the
	// region. This function take a RGNDATA structure on entry. We will add rectangles by
	// amount of ALLOC_UNIT number in this structure.
#define ALLOC_UNIT   100	
	DWORD maxRects = ALLOC_UNIT;
	HANDLE hHeap = HeapCreate( HEAP_NO_SERIALIZE, 0, 0 );
	if (  hHeap == NULL )
	{
		DeleteObject( SelectObject( hMemDC, hDIBBmp ) );
		DeleteDC( hMemDC );
		return NULL;
	}
	RGNDATA* pData = (RGNDATA *)HeapAlloc( hHeap, HEAP_NO_SERIALIZE|HEAP_ZERO_MEMORY, sizeof(RGNDATAHEADER) + sizeof(RECT) * maxRects );
	if ( pData == NULL )
	{
		DeleteObject( SelectObject( hMemDC, holdBmp ) );
		DeleteDC( hMemDC );
		HeapDestroy( hHeap );
		return NULL;
	}
	pData->rdh.dwSize = sizeof( RGNDATAHEADER );
	pData->rdh.iType = RDH_RECTANGLES;
	pData->rdh.nCount = 0;
	pData->rdh.nRgnSize = 0;

	//region的边界值的初始值，都设置为极限值
	SetRect( &pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0 );

	//透明颜色的范围
	BYTE lowRed = GetRValue( cTransparentColor );
	BYTE lowGreen = GetGValue( cTransparentColor );
	BYTE lowBlue = GetBValue( cTransparentColor );
	BYTE hightRed = min( 0xff, lowRed + GetRValue( cTolerance) );
	BYTE hightGreen = min( 0xff, lowGreen + GetGValue( cTolerance) );
	BYTE hightBlue = min( 0xff, lowBlue + GetBValue( cTolerance) );

	// 遍历位图，从内存底部到顶部，因为位图是反转保存的
	BYTE *pBitMap = (BYTE *)bmDIB.bmBits + (bmDIB.bmHeight - 1) * bmDIB.bmWidthBytes;
	for ( int yIndex = 0; yIndex < bmDIB.bmHeight; ++yIndex )
	{
		// 从左到右遍历
		for ( int xIndex = 0; xIndex < bmDIB.bmWidth; ++xIndex )
		{
			//寻找一块不透明区域
			int xBegin = xIndex;
			LONG *pWidth = (LONG *)pBitMap + xIndex;   //指向本次判断开始位置
			while ( xIndex < bmDIB.bmWidth )
			{
				BYTE bColor = GetRValue( *pWidth );
				if ( bColor >= lowRed && bColor <= hightRed )
				{
					bColor = GetGValue( *pWidth );
					if ( bColor >= lowGreen && bColor <= hightGreen )
					{
						bColor = GetBValue( *pWidth );
						if ( bColor >= lowBlue && bColor <= hightBlue )
							//透明区域
							break;
					}
				}
				++pWidth;
				++xIndex;
			}
			//非透明区域
			if ( xIndex > xBegin )
			{
				// 从(xBegin, y) 到 (x, y+1) 是非透明区域，加到region中
				if ( pData->rdh.nCount >= maxRects )
				{
					maxRects += ALLOC_UNIT;
					LPVOID pTemp = HeapReAlloc( hHeap, HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY, pData, sizeof(RGNDATAHEADER) + sizeof(RECT) * maxRects );
					if ( pTemp != NULL )
					{
						pData = (RGNDATA*)pTemp;
					}
					else
					{
						DeleteObject( SelectObject( hMemDC, holdBmp ) );
						DeleteDC( hMemDC );
						HeapDestroy( hHeap );
						return NULL;
					}
				}
				RECT *pr = (RECT *)&pData->Buffer;
				//保存需要保留的矩形区域坐标
				SetRect( &pr[pData->rdh.nCount], xBegin, yIndex, xIndex, yIndex+1 );
				++pData->rdh.nCount;

				//重新设定异形区域的边界
				if ( xBegin < pData->rdh.rcBound.left )
					pData->rdh.rcBound.left = xBegin;
				if ( yIndex < pData->rdh.rcBound.top )
					pData->rdh.rcBound.top = yIndex;
				if ( xIndex > pData->rdh.rcBound.right )
					pData->rdh.rcBound.right = xIndex;
				if ( yIndex + 1 > pData->rdh.rcBound.bottom )
					pData->rdh.rcBound.bottom = yIndex + 1;

				// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
				// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
				if (pData->rdh.nCount == 2000)
				{
					//把数据转成Region
					HRGN hRGNTemp = ExtCreateRegion( NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * pData->rdh.nCount ), pData );
					if ( hRgn )
					{
						CombineRgn( hRgn, hRgn, hRGNTemp, RGN_OR );
						DeleteObject( hRGNTemp );
					}
					else
						hRgn = hRGNTemp;
					pData->rdh.nCount = 0;
					SetRect( &pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
				}
			}
		}//for
		//下一行
		pBitMap -= bmDIB.bmWidthBytes;
	}//for
	// Create or extend the region with the remaining rectangles
	HRGN hRGNTemp = ExtCreateRegion( NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * pData->rdh.nCount ), pData );
	if ( hRgn )
	{
		CombineRgn( hRgn, hRgn, hRGNTemp, RGN_OR );
		DeleteObject( hRGNTemp );
	}
	else
		hRgn = hRGNTemp;


	DeleteObject( SelectObject( hMemDC, holdBmp ) );
	DeleteDC( hMemDC );
	HeapDestroy( hHeap );
	return hRgn;
}

void CTransparent::OnPaint( HWND hwnd, HDC hdc, const std::wstring& text )
{
	RECT client_rc;
	::GetClientRect(hwnd, &client_rc);
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	HBITMAP hNewBmp = ::CreateCompatibleBitmap(hdc, client_rc.right - client_rc.left, client_rc.bottom - client_rc.top);
	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hNewBmp);

	::SetBkMode(hMemDC, TRANSPARENT);
	::SetTextColor(hMemDC, RGB(255, 250, 250));
	::DrawText(hMemDC, 
		text.c_str(),
		-1, 
		&client_rc, 
		DT_LEFT | DT_NOCLIP | DT_NOPREFIX);

	//一个BITMAP只能被一个DC选中
	HBITMAP hTempBmp = ::CreateCompatibleBitmap(hMemDC, 0, 0);
	HBITMAP hTextBmp = (HBITMAP)::SelectObject(hMemDC, hTempBmp); 
	HRGN hRgn = BitmapToRegion( hTextBmp, RGB(0,0,0), RGB(1,1,1) );
	hTempBmp = (HBITMAP)::SelectObject(hMemDC, hTempBmp);
	::DeleteObject(hTempBmp);
	::SetWindowRgn(hwnd,hRgn, TRUE );
	::BitBlt(hdc, client_rc.left, client_rc.top, client_rc.right - client_rc.left, client_rc.bottom - client_rc.top, hMemDC, client_rc.left, client_rc.top, SRCCOPY);
	::SelectObject(hdc, hOldBmp);
	::DeleteObject(hNewBmp);
	::DeleteDC(hMemDC);
}


#include "bitmap_utility.h"
#include <new.h>

int SaveBitmapToFile( HBITMAP hDDBmap, LPCTSTR lpFileName )
{
	BITMAP       Bitmap; //位图属性结构 
	//计算位图文件每个像素所占字节数 
	HDC hTempDC = ::CreateDC(L"DISPLAY", NULL, NULL, NULL); 
	int iBits = ::GetDeviceCaps(hTempDC, BITSPIXEL) * ::GetDeviceCaps(hTempDC, PLANES); //当前显示分辨率下每个像素所占字节数 
	::DeleteDC(hTempDC); 

	WORD wBitCount = 0; //位图中每个像素所占位数
	if (iBits <= 1) 
		wBitCount = 1; 
	else if (iBits <= 4) 
		wBitCount = 4; 
	else if (iBits <= 8) 
		wBitCount = 8; 
	else if (iBits <= 24) 
		wBitCount = 24; 
	else if (iBits <= 32)
		wBitCount = 32;

	//计算调色板所占空间
	//如果一个像素所占空间小于等于8位，则使用调色板，否则直接存储RGB值
	DWORD dwPaletteSize=0;
	if (wBitCount <= 8) 
	{
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD); 
	}

	 DWORD dwDIBSize, dwWritten; 
	//获取DDB位图信息，然后设置文件位图信息头结构 
	::GetObject(hDDBmap, sizeof(BITMAP), (LPSTR)&Bitmap); 
	BITMAPINFOHEADER bi; //位图信息头结构 
	::memset(&bi, 0, sizeof(BITMAPINFOHEADER));
	bi.biSize = sizeof(BITMAPINFOHEADER); 
	bi.biWidth = Bitmap.bmWidth; 
	bi.biHeight = Bitmap.bmHeight; 
	bi.biPlanes = 1; 
	bi.biBitCount = wBitCount;  //一个像素点占用的位数
	bi.biCompression = BI_RGB; 
	//bi.biSizeImage = 0; 
	//bi.biXPelsPerMeter = 0; 
	//bi.biYPelsPerMeter = 0; 
	//bi.biClrUsed = 0; 
	//bi.biClrImportant = 0; 
	
	DWORD dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;   //向上取整，计字节数
	//为位图内容分配内存 
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)new(std::nothrow) char[dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER)]; //位图信息头结构 
	*lpbi = bi; 
	// 处理调色板   
	HANDLE hOldPal = NULL;
	HDC hDCPalatte = NULL;
	HANDLE hPal = ::GetStockObject(DEFAULT_PALETTE); 
	if (hPal) 
	{ 
		hDCPalatte = ::GetDC(NULL); 
		hOldPal = ::SelectPalette(hDCPalatte, (HPALETTE)hPal, FALSE); 
		::RealizePalette(hDCPalatte); 
	} 
	// 获取该调色板下新的像素值 
	::GetDIBits(hDCPalatte, hDDBmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (LPBITMAPINFO) lpbi, DIB_RGB_COLORS); 
	//恢复调色板   
	if (hOldPal) 
	{ 
		::SelectPalette(hDCPalatte, (HPALETTE)hOldPal, TRUE); 
		::RealizePalette(hDCPalatte); 
		::ReleaseDC(NULL, hDCPalatte); 
	} 

	//创建位图文件   
	HANDLE hFile = ::CreateFile(lpFileName, GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL); 
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		delete [] lpbi;
		return -1; 
	}
	//设置位图文件头
	BITMAPFILEHEADER bmfHdr; //位图文件头结构 
	::memset(&bmfHdr, 0, sizeof(BITMAPFILEHEADER));
	bmfHdr.bfType = 0x4D42;   // "BM " 
	//位图文件大小：位图文件头 +　位图信息头 + 调色板空间 + 位图真实数据空间
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;   
	bmfHdr.bfSize = dwDIBSize; 
	/*bmfHdr.bfReserved1 = 0; 
	bmfHdr.bfReserved2 = 0;*/ 
	//位图真实数据位置：位图文件头 +　位图信息头 + 调色板空间
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; // 写入位图文件头 

	::WriteFile(hFile, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 
	// 写入位图文件其余内容 
	::WriteFile(hFile, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);   

	::CloseHandle(hFile); 
	delete [] lpbi;
	return 0;
}

HANDLE DDBToDIB( HBITMAP bitmap, DWORD dwCompression, HPALETTE Pal )
{
	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS )
		return NULL;
	// Get bitmap information
	BITMAP bm;
	::GetObject(bitmap, sizeof(BITMAP), &bm);

	// Initialize the bitmapinfoheader
	BITMAPINFOHEADER bi;
	::RtlZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression = dwCompression;
	// Compute the size of the infoheader and the color table
	int nColors = 0;
	//一个像素点使用小于等于8位表示，则需要调色板
	if (bi.biBitCount <= 8)
	{
		nColors = bi.biClrUsed ? bi.biClrUsed : 1 << bi.biBitCount;
	}
	DWORD DIBInfoSize;
	DIBInfoSize = bi.biSize + nColors * sizeof(RGBQUAD);
	LPVOID  lpDIB = new(std::nothrow) char[DIBInfoSize];
	if (lpDIB == NULL)
	{
		return NULL;
	}	
	::memcpy(lpDIB, &bi, sizeof(BITMAPINFOHEADER));
	// We need a device context to get the DIB from
	HDC DIBhDC = ::GetDC(NULL);
	// If a palette has not been supplied use defaul palette
	BOOL  bNeedPalette = false;
	if(nColors != 0 && ::GetDeviceCaps(DIBhDC, RASTERCAPS) & RC_PALETTE)
		bNeedPalette = true;
	HPALETTE hOldPal = NULL;
	HPALETTE hPal = Pal;
	if (bNeedPalette)
	{
		if (hPal == NULL)
			hPal = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
		hOldPal = ::SelectPalette(DIBhDC, hPal, FALSE);
		::RealizePalette(DIBhDC);
	}
	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	::GetDIBits(DIBhDC, bitmap, 0L, (DWORD)bi.biHeight,
		(LPBYTE)NULL, (LPBITMAPINFO)lpDIB, (DWORD)DIB_RGB_COLORS);
	::memcpy(&bi, lpDIB, sizeof(BITMAPINFOHEADER));
	delete [] lpDIB;
	lpDIB = NULL;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0)
	{
		//32的倍数
		bi.biSizeImage = (((bi.biWidth * bi.biBitCount + 31)& ~31) / 8) * bi.biHeight;
		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}
	// Realloc the buffer so that it can hold all the bits
	DWORD DIBSize = DIBInfoSize + bi.biSizeImage;
	lpDIB = new(std::nothrow) char[DIBSize];
	if (lpDIB != NULL)
	{
		// FINALLY get the DIB
		::memcpy(lpDIB, &bi, sizeof(BITMAPINFOHEADER));
		BOOL bGotBits = ::GetDIBits(DIBhDC, bitmap,
			0L, // Start scan line
			(DWORD)bi.biHeight, // # of scan lines
			(LPBYTE)lpDIB + DIBInfoSize,// address for bitmap bits
			(LPBITMAPINFO)lpDIB, // address of bitmapinfo
			(DWORD)DIB_RGB_COLORS); // Use RGB for color table

		if(!bGotBits)
		{
			delete [] lpDIB; 
			lpDIB = NULL;;
		}
	}	
	// Reselect the original palette
	if (bNeedPalette && hPal != NULL)
	{
		::SelectPalette(DIBhDC, hOldPal, FALSE);
	}
	::ReleaseDC(NULL, DIBhDC);
	return lpDIB;
}

HBITMAP DIBToDDB( HANDLE hDIB )
{
	if (hDIB == NULL)
		return NULL;
	LPBITMAPINFOHEADER	lpbi;
	HPALETTE		     pal = NULL;
	HPALETTE            pOldPal = NULL;

	lpbi = (LPBITMAPINFOHEADER)hDIB;
	int nColors = 0;
	//每个像素用小于等于8位表示时，才有调色板
	if ( lpbi->biBitCount <= 8)
	{
		nColors = lpbi->biClrUsed ? lpbi->biClrUsed : 1 << lpbi->biBitCount;
	}

	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	LPVOID lpDIBBits;
	if(bmInfo.bmiHeader.biBitCount < 8)
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + nColors*sizeof(RGBQUAD)) + ((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)bmInfo.bmiColors;

	// Create and select a logical palette if needed
	HDC   DDBHdc = NULL;
	DDBHdc = ::GetDC(NULL);
	if( nColors != 0 && ::GetDeviceCaps(DDBHdc, RASTERCAPS ) & RC_PALETTE)
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

		pLP->palVersion = 0x300;
		pLP->palNumEntries = (WORD)nColors;

		for(int i=0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
			pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}

		//pal.CreatePalette( pLP );
		pal = ::CreatePalette(pLP);

		delete[] pLP;

		// Select and realize the palette
		pOldPal = ::SelectPalette(DDBHdc, pal, FALSE);
		::RealizePalette(DDBHdc);
	}
	BITMAPINFO bmi = {0};
	memcpy(&bmi, lpbi, sizeof(BITMAPINFOHEADER));

	HBITMAP hbm = ::CreateDIBitmap( DDBHdc,		// handle to device context
		(LPBITMAPINFOHEADER)lpbi,	// pointer to bitmap info header
		(LONG)CBM_INIT,			// initialization flag
		lpDIBBits,			        // pointer to initialization data
		&bmi,		// pointer to bitmap info
		DIB_RGB_COLORS );		    // color-data usage

	if (pal)
		SelectPalette(DDBHdc, pOldPal, FALSE);

	return hbm;
}

HBITMAP TransBitmap( HBITMAP hbm )
{
	HDC hDCA = ::CreateDC( L"DISPLAY", 0, 0, 0 );
	int nBitsColor = ::GetDeviceCaps(hDCA, BITSPIXEL);
	int nBitCount = nBitsColor * ::GetDeviceCaps(hDCA, PLANES); //每个像素的bit
	::DeleteDC(hDCA);

	BITMAP bm;
	::GetObject(hbm, sizeof(BITMAP), &bm);
	//填充 BITMAP头
	BITMAPINFOHEADER bmih = {0};
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biBitCount = (WORD)nBitCount;
	bmih.biWidth = bm.bmWidth;
	bmih.biHeight = bm.bmHeight;
	bmih.biPlanes = 1;

	int nColors = 0;
	if (bmih.biBitCount <= 8)
	{
		nColors = bmih.biClrUsed ? bmih.biClrUsed : 1 << bmih.biBitCount;
	}

	int nDIBHeadSize = sizeof(BITMAPINFOHEADER) + nColors * sizeof(RGBQUAD);
	int nbitSize = ((bm.bmWidth * nBitCount + 31) & ~31) / 8 * bmih.biHeight + nDIBHeadSize;   // bi.biSizeImage是DWORD的倍数

	LPVOID lpDIB = new(std::nothrow) char[nbitSize];  
	if (lpDIB != NULL)
	{
		RtlZeroMemory(lpDIB, nbitSize);
		memcpy( lpDIB, &bmih, sizeof(BITMAPINFOHEADER) );
		//设置调色板，大于256色的DIB图是没有调色板的
		HDC DIBHdc = ::GetDC( NULL );
		BOOL  bNeedPalette = false;
		if( nColors != 0 && ::GetDeviceCaps(DIBHdc, RASTERCAPS) & RC_PALETTE)
			bNeedPalette = true;
		HPALETTE hOldPalette = NULL;
		if (bNeedPalette)
		{
			HPALETTE hPalette = (HPALETTE)::GetStockObject(DEFAULT_PALETTE);
			if (hPalette)
			{
				hOldPalette = ::SelectPalette(DIBHdc, hPalette, false);
				::RealizePalette(DIBHdc);
			}
		}

		//DDB转DIB
		::GetDIBits(DIBHdc, hbm, 0, bmih.biHeight, (char*)lpDIB + nDIBHeadSize, (LPBITMAPINFO)lpDIB, DIB_RGB_COLORS);

		if ( bNeedPalette && hOldPalette )
		{
			::SelectPalette(DIBHdc, hOldPalette, true);
			::RealizePalette(DIBHdc) ;
		}

		::ReleaseDC(NULL, DIBHdc);

		//DIB转换成DDB
		BITMAPINFO pbmi = {0};
		pbmi.bmiHeader.biSize = 40;
		pbmi.bmiHeader.biWidth = bm.bmWidth;
		pbmi.bmiHeader.biHeight = bm.bmHeight;
		pbmi.bmiHeader.biPlanes = 1;
		pbmi.bmiHeader.biBitCount = (WORD)nBitCount;

		HDC hdc = ::GetDC(NULL);
		HBITMAP hbmp = CreateDIBitmap(hdc ,&bmih, CBM_INIT, (char*)lpDIB + nDIBHeadSize, &pbmi, DIB_RGB_COLORS);
		delete[] lpDIB;
		::ReleaseDC(NULL, hdc);
		return hbmp;
	}
	else
		return NULL;
}

void TestBitMap( HWND hWnd )
{
	HDC hdc = ::GetDC(hWnd);
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	HBITMAP hMemBmp = ::CreateCompatibleBitmap(hdc, 200, 20);
	::ReleaseDC(hWnd, hdc);
	HBITMAP hBmpBak = (HBITMAP)::SelectObject(hMemDC, hMemBmp);
	::SetBkMode(hMemDC, TRANSPARENT);
	HBRUSH hBrush = ::CreateSolidBrush(RGB(0,255,25));
	RECT text_rc = {0,0,200,20};
	::FillRect(hMemDC, &text_rc, hBrush);
	::SetTextColor(hMemDC, RGB(255, 2, 40));
	::DrawText(hMemDC, L"hello cswuyg 2013-4-14", -1, &text_rc, DT_LEFT | DT_NOCLIP | DT_NOPREFIX);
	::SelectObject(hMemDC, hBmpBak);
	HBITMAP hCopy_1 = TransBitmap(hMemBmp);
	::DeleteObject(hMemBmp);
	::DeleteDC(hMemDC);
	SaveBitmapToFile(hCopy_1, L"C:\\copy_1.bmp");
	HBITMAP h_Copy_bit = (HBITMAP)DDBToDIB(hCopy_1, BI_RGB, NULL);
	::DeleteObject(hCopy_1);
	HBITMAP hCopy_2 = DIBToDDB(h_Copy_bit);
	delete [] h_Copy_bit;
	SaveBitmapToFile_2(hCopy_2, L"C:\\copy_2.bmp");
	::DeleteObject(hCopy_2);


	//::StretchBlt(hMemDC,text_rc.left, text_rc.top, 100, 20, hdc, text_rc.left, text_rc.top, 100, 20,SRCPAINT);
	//HBITMAP hxx = (HBITMAP)::LoadImage(NULL, L"c:\\temp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void SaveBitmapToFile_2( HBITMAP hBitmap, LPCTSTR lpFileName )
{
	HANDLE hBIT =  DDBToDIB(hBitmap, BI_RGB , NULL); //这个返回值其实是new出来的数据，后面必须delete
	LPBITMAPINFO lpBitInfo = (LPBITMAPINFO)hBIT;

	DWORD dwPaletteSize = 0;  //调色板的颜色数量，也就是RGBQUAD结构体的个数
	if (lpBitInfo->bmiHeader.biBitCount <= 8)
	{
		if (lpBitInfo->bmiHeader.biClrUsed == 0)
		{
			dwPaletteSize = (1 << lpBitInfo->bmiHeader.biBitCount) * sizeof(RGBQUAD);
		}
		else
		{
			dwPaletteSize = lpBitInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD);
		}
	}
	DWORD  dwImageSize = ( ( lpBitInfo->bmiHeader.biWidth * lpBitInfo->bmiHeader.biBitCount + 31 )& ~31) / 8 * lpBitInfo->bmiHeader.biHeight;

	// 设置位图文件头 
	BITMAPFILEHEADER   bmfHdr; 
	bmfHdr.bfType = 0x4D42;   // "BM " 
	bmfHdr.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwImageSize; 
	bmfHdr.bfReserved1 = 0; 
	bmfHdr.bfReserved2 = 0; 
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize; // 写入位图文件头 

	//这里使用WriteFile也可以，我用CreateFileMapping，只是为了测试内存映射文件的写入
	HANDLE hNewFile = ::CreateFile(lpFileName, GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
	if (hNewFile == INVALID_HANDLE_VALUE) 
		return;

	HANDLE hFileMap =  ::CreateFileMapping(hNewFile, NULL, PAGE_READWRITE, 0, bmfHdr.bfSize, NULL);
	LPVOID lpFile = ::MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, bmfHdr.bfSize);

	// 写入位图文件其余内容 
	memcpy(lpFile, &bmfHdr, sizeof(BITMAPFILEHEADER));
	memcpy((char*)((char*)lpFile+sizeof(BITMAPFILEHEADER)), (char*)hBIT, bmfHdr.bfSize-sizeof(BITMAPFILEHEADER));
	UnmapViewOfFile(lpFile);
	CloseHandle(hFileMap);
	CloseHandle(hNewFile);
	delete [] lpBitInfo;
}


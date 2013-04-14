#pragma once
#include <windows.h>
/**
本例子是对位图的使用
cswuyg@gmail.com
2014.4.14
*/
/**
DIB设备无关位图 
DDB设备相关位图 [compatible bitmap]

《数字图像处理入门》一书，对位图格式有详细介绍。
*/

/**
DDB位图转为DIB位图，返回值其实是一块new出来的内存区域，使用后需要delete
流程：通过GetObject获得DDB位图信息+调色板信息填充到BITMAPINFO，然后用GetDIBits填充位图数据
*/
HANDLE DDBToDIB( HBITMAP bitmap, DWORD dwCompression, HPALETTE Pal );

/**
DIB位图转DDB位图
流程：拷贝调色板，调用CreateDIBitmap，从DIB创建一个DDB
*/
HBITMAP DIBToDDB( HANDLE hDIB );


/**
DDB转DIB，DIB再转DDB的测试
*/
HBITMAP TransBitmap(HBITMAP hbm);

/*
保存位图到文件
流程：
1、通过位图句柄得到位图大小，填充位图文件头、位图信息头、位图调色板空间（如果有的话），
2、通过GetDIBits把DDB位图数据读取到内存
3、把位图文件头、位图信息头+位图调色板空间+位图数据写入到文件
param hDDBmap DDB位图句柄
param lpFileName 保存的文件名
*/
int SaveBitmapToFile(HBITMAP hDDBmap , LPCTSTR lpFileName);

/**
保存位图都文件的第二种实现。
位图文件其实就是：位图文件头 +　位图的DIB格式（包括了BITMAPFILEHEADER、调色板空间、位图实际数据），
所以可以先试用DDBToDIB，得到DIB格式，然后再拼出一个位图文件头，接着把数据写入文件。
*/
void SaveBitmapToFile_2( HBITMAP hBitmap, LPCTSTR lpFileName );

/**
对上面4个函数的使用进行测试
*/
void TestBitMap(HWND hWnd);




// MergeBigFile.cpp : Defines the entry point for the console application.
//

#include "utility.h"
#include "CalcCostTime.h"

#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <iostream>

/**
在对文件做读取、写入处理的时候内存映射文件会比使用new/fopen快。
可能是系统为内存映射文件根据局部性原理做了优化的缘故。
cswuyg
2013.6.5
*/


//const wchar_t* const kSourceFilePath = L"D:\\gnet_temp\\cswuyg_source_big.zip";
//const wchar_t* const kNewFilePath = L"D:\\gnet_temp\\cswuyg_dest.zip";


const wchar_t* const kSourceFilePath = L"D:\\gnet_temp\\cswuyg_a.rar";
const wchar_t* const kNewFilePath = L"D:\\gnet_temp\\cswuyg_b.rar";


const int kReadBlock = 100 * 1024 * 1024;         

using namespace utility;
using namespace WYGFile;

enum E_freadError
{
	e_suc,
	e_fail_file_end,
	e_fail_stream_error,
};

E_freadError CheckfreadError(FILE* fp_read)
{
	if (ferror(fp_read) == 0)
	{
		::OutputDebugString(L"fread_s error, stream no error\r\n");
	}
	else
	{
		::OutputDebugString(L"fread_s error, stream error \r\n");
		return e_fail_stream_error;
	}
	if (feof(fp_read) == 0)
	{
		::OutputDebugString(L"fread_s error, stream no read end\r\n");
	}
	else
	{
		::OutputDebugString(L"fread_s error, stream read end\r\n");
		return e_fail_file_end;
	}
	return e_suc;
}

void Testfopen()
{
	CCostTime cost_time;
	FILE* fp_read = NULL;
	FILE* fp_write = NULL;
	errno_t e_read_ret = _wfopen_s(&fp_read, kSourceFilePath, L"rb");  // 必须有b否则fread_s出错
	errno_t e_write_ret = _wfopen_s(&fp_write, kNewFilePath, L"wb");
	if (e_read_ret != 0 || e_write_ret != 0)
	{
		::OutputDebugString(L"_wfopen_s error\r\n");
	}
	else
	{
		_fseeki64(fp_read, 0, SEEK_END);
		LONGLONG llFileSize = _ftelli64(fp_read);
		_fseeki64(fp_read, 0, SEEK_SET);

		_fseeki64(fp_write, 0, SEEK_SET);
		LONGLONG nTotalRead = 0;
		char* buf = new char[kReadBlock];
		bool bSuc = true;
		while ((llFileSize > kReadBlock) && nTotalRead <= (llFileSize - kReadBlock))
		{
			size_t nReadSize =  fread_s(buf, kReadBlock, kReadBlock, 1, fp_read);  
			if (nReadSize != 1)
			{
				CheckfreadError(fp_read);
				bSuc = false;
				break;
			}
			size_t nWriteSize = fwrite(buf, kReadBlock, 1, fp_write);
			if (nWriteSize != 1)
			{
				::OutputDebugString(L"fwrite error\r\n");
				bSuc = false;
				break;
			}
			nTotalRead +=  kReadBlock;
		}
		if (bSuc)
		{
			do 
			{
				size_t nReadSize =  fread_s(buf, kReadBlock, static_cast<size_t>(llFileSize - nTotalRead), 1, fp_read);
				LONGLONG llPos = _ftelli64(fp_read);
				if (nReadSize != 1)
				{
					CheckfreadError(fp_read);
					bSuc = false;
					break;
				}
				size_t nWriteSize = fwrite(buf, static_cast<size_t>(llFileSize - nTotalRead), 1, fp_write);
				if (nWriteSize != 1)
				{
					bSuc = false;
				}
			} while (false);
		}
		
		delete [] buf;
		fclose(fp_read);
		fclose(fp_write);

		std::wcout << L"Test fopen Cost Time : " << cost_time.CostTime() << L" second"<< std::endl;
	}
}

void TestMemMap()
{
	CCostTime cost_time;

	CMemoryMappedFile mem_file_read(kSourceFilePath, GENERIC_READ, OPEN_EXISTING, PAGE_READONLY, 0, 0, FILE_MAP_READ);
	bool bRead = mem_file_read.IsValid();
	if (!bRead)
	{
		return;
	}
	_LARGE_INTEGER size;
	size.QuadPart = mem_file_read.length();
	CMemoryMappedFile mem_file_write(kNewFilePath, GENERIC_READ | GENERIC_WRITE, OPEN_ALWAYS, PAGE_READWRITE, size.HighPart, size.LowPart, FILE_MAP_WRITE);
	bool bWrite = mem_file_write.IsValid();
	if (!bWrite)
	{
		return;
	}
	LONGLONG read_file_size = mem_file_read.length();
	if (read_file_size < 100 * 1024 * 1024)
	{
		char* pRead = mem_file_read.MapView(0, 0, 0);
		char* pWrite = mem_file_write.MapView(0, 0, 0);
		errno_t eCpyRet = memcpy_s(pWrite, static_cast<rsize_t>(read_file_size), pRead, static_cast<rsize_t>(read_file_size));
		if (eCpyRet != 0)
		{
			::OutputDebugString(L"memcpy_s error\r\n");
		}
		//for (LONGLONG i = 0; i != read_file_size; ++i)
		//{
		//	*(pWrite + i) = *(pRead + i);
		//}
	}
	else
	{
		unsigned int nBlock = 100 * 1024 * 1024;
		_LARGE_INTEGER big_pos;
		big_pos.QuadPart = 0;
		while(big_pos.QuadPart <= (read_file_size - nBlock))
		{
			char* pRead = mem_file_read.MapView(big_pos.HighPart, big_pos.LowPart, nBlock);
			char* pWrite = mem_file_write.MapView(big_pos.HighPart, big_pos.LowPart, nBlock);
			errno_t eCpyRet = memcpy_s(pWrite, nBlock, pRead, nBlock);
			if (eCpyRet != 0)
			{
				::OutputDebugString(L"memcpy_s error\r\n");
			}
			//逐个字节的拷贝，速度极慢
			//for (int i = 0; i != nBlock; ++i)
			//{
			//	*(pWrite + i) = *(pRead + i);
			//}
			big_pos.QuadPart += nBlock;
		}
		unsigned int nLastLength = static_cast<unsigned int>(read_file_size - big_pos.QuadPart);
		char* pRead = mem_file_read.MapView(big_pos.HighPart, big_pos.LowPart, nLastLength);
		char* pWrite = mem_file_write.MapView(big_pos.HighPart, big_pos.LowPart, nLastLength);
		errno_t eCpyRet = memcpy_s(pWrite, nLastLength, pRead, nLastLength);
		if (eCpyRet != 0)
		{
			::OutputDebugString(L"memcpy_s error\r\n");
		}
		//逐个字节的拷贝，速度极慢
		//for (int i = 0; i != nLastLength; ++i)
		//{
		//	*(pWrite + i) = *(pRead + i);
		//}
	}

	std::wcout << L"TestMemMap Cost Time : " << cost_time.CostTime() << L" second"<< std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	Testfopen();
	TestMemMap();
	system("pause");
	return 0;
}

/**
FileSize = 300+MB 
result:

TestMemMap();
Testfopen();
TestMemMap Cost Time : 0.294138 second
Test fopen Cost Time : 3.33694 second
请按任意键继续. . .


Test fopen Cost Time : 3.24294 second
TestMemMap Cost Time : 0.293308 second
请按任意键继续. . .
*/

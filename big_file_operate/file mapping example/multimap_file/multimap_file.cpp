// multimap_file.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#include "mapview_file.h"

const wchar_t* const kSourceFilePath = L"D:\\gnet_temp\\cswuyg_bb.rar";
const wchar_t* const kDestFilePath = L"D:\\gnet_temp\\cswuyg_aa.rar";

const int kDestFileSize = 64 * 3 * 1024;
const wchar_t* const kFileMappingWriteName = L"cswuyg_write_file_map";
const wchar_t* const kFileMappingReadName = L"cswuyg_read_file_map";

//void testMultiMap()
//{
//	HANDLE hFile = ::CreateFile(kSourceFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (hFile == INVALID_HANDLE_VALUE)
//	{
//		return;
//	}
//	HANDLE hMap_1 = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, kDestFileSize, L"Local\\cswuyg_example");
//	if (hMap_1 == NULL)
//	{
//		DWORD dwError = ::GetLastError();
//		if (dwError == ERROR_INVALID_HANDLE)
//		{
//			::OutputDebugString(L"Same Name error");
//		}
//	}
//	HANDLE hMap_2 = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, kDestFileSize, L"Local\\cswuyg_example");
//	if (hMap_2 == NULL)
//	{
//		DWORD dwError = ::GetLastError();
//		if (dwError == ERROR_INVALID_HANDLE)
//		{
//			::OutputDebugString(L"Same Name error");
//		}
//	}
//	char* pData_0 = static_cast<char*>(::MapViewOfFile(hMap_1, FILE_MAP_WRITE, 0, 0, 64 * 1024));
//	char* pData_1 = static_cast<char*>(::MapViewOfFile(hMap_1, FILE_MAP_WRITE, 0, 64 * 1024, 64 * 1024));
//	char* pData_2 = static_cast<char*>(::MapViewOfFile(hMap_2, FILE_MAP_WRITE, 0, 128 * 1024, 64 * 1024));
//
//	*(pData_0) = '1';
//	*(pData_0 + 1) = '2';
//	*(pData_0 + 3) = '3';
//	
//
//	*(pData_1) = '4';
//	*(pData_1 + 1) = '5';
//	*(pData_1 + 3) = '6';
//
//	*(pData_2) = '7';
//	*(pData_2 + 1) = '8';
//	*(pData_2 + 3) = '9';
//
//	::UnmapViewOfFile(pData_0);
//	::UnmapViewOfFile(pData_1);
//	::UnmapViewOfFile(pData_2);
//	::CloseHandle(hMap_1);
//	::CloseHandle(hMap_2);
//	::CloseHandle(hFile);
//}

void testMap()
{
	CMultiMappedFile map_write_base_file(kDestFilePath, GENERIC_READ | GENERIC_WRITE, OPEN_ALWAYS, PAGE_READWRITE, 0, 3 * 64 * 1024, FILE_MAP_WRITE, kFileMappingWriteName);
	CMultiMappedFile map_write_0(kFileMappingWriteName, FILE_MAP_WRITE);
	CMultiMappedFile map_write_1(kFileMappingWriteName, FILE_MAP_WRITE);
	CMultiMappedFile map_write_2(kFileMappingWriteName, FILE_MAP_WRITE);
	char* write_data_0 = map_write_0.MapView(0, 0, 64 * 1024);
	char* write_data_1 = map_write_1.MapView(0, 64 * 1024, 64 * 1024);
	char* write_data_2 = map_write_2.MapView(0, 128 * 1024, 64 * 1024);


	CMultiMappedFile map_read_base_file(kSourceFilePath, GENERIC_READ | GENERIC_WRITE, OPEN_ALWAYS, PAGE_READWRITE, 0, 3 * 64 * 1024, FILE_MAP_WRITE, kFileMappingReadName);
	
	char* read_data_0 = map_read_base_file.MapView(0, 0, 64 * 1024);
	errno_t eCpyRet = memcpy_s(write_data_0, 64 * 1024, read_data_0, 64 * 1024);
	if (eCpyRet != 0)
	{
		::OutputDebugString(L"memcpy_s error\r\n");
	}

	char* read_data_1 = map_read_base_file.MapView(0, 64 * 1024, 64 * 1024);
	eCpyRet = memcpy_s(write_data_1, 64 * 1024, read_data_1, 64 * 1024);
	if (eCpyRet != 0)
	{
		::OutputDebugString(L"memcpy_s error\r\n");
	}

	char* read_data_2 = map_read_base_file.MapView(0, 128 * 1024, 64 * 1024);
	 eCpyRet = memcpy_s(write_data_2, 64 * 1024, read_data_2, 64 * 1024);
	if (eCpyRet != 0)
	{
		::OutputDebugString(L"memcpy_s error\r\n");
	}

	map_write_0.Destroy();
	map_write_1.Destroy();
	map_write_2.Destroy();
	map_write_base_file.Destroy();

	map_read_base_file.Destroy();

}

int _tmain(int argc, _TCHAR* argv[])
{
	//testMultiMap();
	testMap();
	system("pause");
	return 0;
}


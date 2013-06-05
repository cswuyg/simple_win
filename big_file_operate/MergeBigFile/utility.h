#pragma once
/*
2012-12-10 始
cswuyg@gmail.com
*/
#include <string>
#include <windows.h>
#define IF_BREAK(x) if (x) break;


namespace utility
{
	/**文件相关*/
	namespace WYGFile
	{
	
		//获取文件大小
		LONGLONG GetFileSize(const std::wstring& strFilePath);
		class  CMemoryMappedFile 
		{
		public:
			CMemoryMappedFile(const std::wstring& strPath, DWORD CreateFile_dwDesiredAccess, DWORD dwCreationDisposition, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, DWORD MapView_dwDesiredAccess);
			~CMemoryMappedFile();;

			bool Initialize();  
			char* MapView(DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap);

			LONGLONG length() const;
			bool IsValid() const;

		private:
			char* m_data;
			LONGLONG m_llFileSize;
			HANDLE m_hMap;
			DWORD m_dwDesiredAccess;
		};
	}

}

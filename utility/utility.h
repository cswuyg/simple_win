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
		//读取文件 
		BOOL WriteToDiskA(const std::wstring& strFilePath, std::string& data);
		//读取文件(小于1G，大文件需要使用内存映射)
		BOOL ReadFromDiskA(const std::wstring& strFilePath, std::string& data);
		bool DeleteDirectory(const std::wstring& strFolder, bool bDelRootFolder);	
		//获取可写的固定磁盘且有最大磁盘空间的盘符
		wchar_t GetMaxFreeCanWriteDiskID(ULONGLONG& freeSpace);    
		//获取路径下的空余空间
		unsigned int GetPathFreeSpace(const std::wstring& strPath); 
		//获取文件大小
		LONGLONG GetFileSize(const std::wstring& strFilePath);
		 //遇到空文件则删除，其他情况(非空文件、删除失败)则重命名..(重命名之后递归)
		bool RenameAndDelTempFile(std::wstring& strDest, const std::wstring& strSource);
		//文件存在则删除，删除失败则重命名，重命名之后还存在则删除，删除失败.则...（递归）
		bool RenameAndDelFile(std::wstring& strDest, const std::wstring& strSource);
		//打开文件所在文件夹并选中文件
		bool OpenPathAndSelectFile(const std::wstring& strPath);
		//内存映射，用于处理大文件
		class  CMemoryMappedFile 
		{
		public:
			CMemoryMappedFile(const std::wstring& strPath, DWORD CreateFile_dwDesiredAccess, DWORD dwCreationDisposition, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, DWORD MapView_dwDesiredAccess);
			~CMemoryMappedFile();;

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

	/**字符串处理相关*/
	namespace WYGString
	{
		std::wstring int2wstr(int nData);
		std::string wstr2str(const std::wstring& strInput, int nEncode);
		std::wstring str2wstr(const std::string& strInput, int nEncode);
	}
	
	/**应用程序相关*/
	namespace WYGAPP
	{
		std::wstring GetAppFullName();
		std::wstring GetModuleFullName();
	}
	
	/**系统相关*/
	namespace WYGSys
	{
		/**获取环境变量*/
		std::wstring GetEnvironmentVariable(const std::wstring& key_name);
		/**添加环境变量某个key的value*/
		void AddEnvironmentVariable(const std::wstring& key_name, const std::wstring& add_value);
	}

	/**编码相关*/
	namespace WYGPageCode
	{
		enum ePageCode
		{
			e_ANSI,         //ANSI编码
			e_UNICODE,      //UNICODE
			e_UTF8,         //UTF8
			e_UTF8_NOBOM,   //UTF8 无标志
			e_BigEnd        //大端 无标志 
		};
		bool IsNoBomUTF8Data(char* content, int length);
		ePageCode GetPageCode(char* content, int length);
	}

	/**位图相关*/
	namespace WYGBmp
	{
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
	}
	
	
	namespace WYGNet
	{
		/**
		如果Default Browser存在则使用它打开url，否则使用ie
		*/
		bool OpenUrlByDefBrowser(const std::wstring& strUrl);
	}

	namespace WYGTime
	{

		std::wstring GetNowUTCTime();
	}

}

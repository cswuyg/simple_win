
/**
测试磁盘相关API
cswuyg@gmail.com
2013.5.3
*/

#include <iostream>
#include "iomanip"
#include <windows.h>

void ShowAllDisk()
{
	DWORD dwMaskdriver = ::GetLogicalDrives();
	if(0 == dwMaskdriver)
		return;
	wchar_t	szDriver[4] = L"A:\\";
	wchar_t wchDriverID = L'\0';
	for(int i = 0; i < 26; ++i)
	{
		if(0 == (dwMaskdriver >> i))
			break;
		if((dwMaskdriver >> i) & 1)
		{
			szDriver[0]= (wchar_t)(i + L'A');
			if (!::GetVolumeInformation(szDriver, 0, 0, NULL,NULL, 0, NULL, 0)) 
				continue;
			ULARGE_INTEGER freeAv;
			ULARGE_INTEGER totalBytes;
			ULARGE_INTEGER freeBytes;
			if (!::GetDiskFreeSpaceEx(szDriver, &freeAv, &totalBytes, &freeBytes))
				continue;
			float nTotal = (float)((float)totalBytes.QuadPart / (ULONGLONG)(1024*1024*1024));
			float nFree = (float)((float)freeBytes.QuadPart / (ULONGLONG)(1024*1024*1024));
			std::wcout << szDriver << "   Total: " \
				<< std::setiosflags(std::ios::fixed) << std::setprecision(2) \
				<< nTotal << L"G" << std::endl << " " << "Left: " \
				<< std::setprecision(2) << nFree << L"G" << std::endl;
		}
	}
}


int main(int argc, char* argv[])
{
	ShowAllDisk();
	system("pause");
	return 0;
}



//DWORD dwDriverType = ::GetDriveType(szDriver);
//int a = 0;
//switch (dwDriverType)
//{
//case DRIVE_UNKNOWN: //未知的磁盘类型
//	a = 1;
//	break;
//case DRIVE_NO_ROOT_DIR: //lpRootPathName是无效的
//	a = 2;
//	break;
//case DRIVE_REMOVABLE: //可移动磁盘
//	a = 3;
//	break;
//case DRIVE_FIXED: //固定磁盘
//	a = 4;
//	break;
//case DRIVE_REMOTE: //网络磁盘
//	a = 5;
//	break;
//case DRIVE_CDROM://光驱
//	a = 6;
//	break;
//case DRIVE_RAMDISK: //内存磁盘
//	a = 7;
//	break;
//}
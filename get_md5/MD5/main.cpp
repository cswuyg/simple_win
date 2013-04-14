#include "MD5Checksum.h"

#include <iostream>
#include <string>

/**
calc the md5 
be used in windows program 
the MD5Checksum.h/cpp come from internet.

cswuyg
*/

std::string GetMD5(const ATL::CStringA& strInput)
{
	CMD5Checksum objMd5;
	ATL::CStringA strMd5 = objMd5.GetMD5((BYTE*)(LPCSTR)strInput, strInput.GetLength());
	return (const char*)strMd5;
}

int main()
{
	std::cout << "calc MD4 " << std::endl;
	std::string strInput;
	std::cin >> strInput;
	std::string strRet = GetMD5(strInput.c_str());
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "MD5 is :" << strRet << std::endl;
	system("pause");
	return 1;
}
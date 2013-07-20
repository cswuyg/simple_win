/**
An example about how to use zlib.

cswuyg
2013.7.18
*/
#define ZLIB_WINAPI
#include <zlib.h>

#include <string>
#include <iostream>
#include <windows.h>
#include <assert.h>


std::string wstr2str(const std::wstring& strInput, int nEncode)
{
	if (strInput.empty())
	{
		return "";
	}
	int nLength = ::WideCharToMultiByte(nEncode, 0, strInput.c_str(), -1, 0, 0, 0, 0);
	char* buf = new char[nLength + 1];
	::memset(buf, 0, nLength + 1);
	::WideCharToMultiByte(nEncode, 0, strInput.c_str(), -1, buf, nLength, 0, 0);
	std::string strResult = buf;
	delete[] buf;
	return strResult;
}

/**
can know the size of compress buffer befoe call the compress api, use the api-compressBound.
can not know the size of uncompress buffer before call the uncompress api, the size should be save at somewhere after compress.
*/
void test_compress(const std::wstring& strInputData)
{
	std::string strCompress;
	std::string strInput = wstr2str(strInputData, CP_ACP);
	{
		std::cout << "Before compress : " << strInput << "      size:" << strInput.size() << std::endl;
		unsigned long nOutSize =  compressBound(strInputData.size());
		char* pOut = new char[nOutSize+1];
		if (compress((unsigned char*)pOut, &nOutSize, (const unsigned char*)strInput.c_str(), strInput.size()) != Z_OK)
		{
			assert(false);
			return;
		}
		std::string strOutData(pOut, nOutSize);
		strCompress = strOutData;
		std::cout << "After compress : " << strCompress << "      size:" << strCompress.size() << std::endl;
	}
	{
		std::cout << "Before uncompress : " << strCompress << "      size:" << strCompress.size() << std::endl;
		char* pOut = new char[strInput.size()+1];
		unsigned long nOutSize = strInput.size();
		if (uncompress((unsigned char*)pOut, &nOutSize, (const unsigned char*)strCompress.c_str(), strCompress.size()) != Z_OK)
		{
			assert(false);
			return;
		}
		std::string strOutData(pOut, nOutSize);
		std::cout << "After uncompress : " << strOutData << "      size" << strOutData.size() << std::endl;
	}
}


int main(int argc, char* argv[])
{
	test_compress(L"test Input Data cswuyg cswuyg cswuyg");
	system("pause");
	return 0;
}


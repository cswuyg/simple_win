/**
openssl lib encryption example

cswuyg
2013.7.12
*/

#pragma once
#include <string>

namespace Encryption
{
	extern const char* kRSAPubKey;
	extern const char* kRSAPriKey;
	extern const char* kRSAPubKeyFileName;
	extern const char* kRSAPriKeyFileName;

	std::string EncodeRSAKeyFile(const std::string& strPemFileName, const std::string& strData);
	std::string DecodeRSAKeyFile(const std::string& strPemFileName, const std::string& strData);
	std::string EncodeRSAKeyStr(const std::string& strKey, const std::string& strData);
	std::string DecodeRSAKeyStr(const std::string& strKey, const std::string& strData);
	std::string EncodeAES(const std::string& strPassword, const std::string& strData);
	std::string DecodeAES(const std::string& strPassword, const std::string& strData);
}
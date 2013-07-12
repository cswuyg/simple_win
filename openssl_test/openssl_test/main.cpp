#include <iostream>
#include <string>
#include "encryption/encryption.h"
#include <openssl/rand.h>

#pragma comment(lib, "libeay32.lib")


void test_rsa_file_key()
{
	std::string strNaked = "I am cswuyg.";
	std::cout << "naked:  " << strNaked << std::endl;
	std::string strEncode = Encryption::EncodeRSAKeyFile(Encryption::kRSAPubKeyFileName, strNaked);
	std::cout << "RSAEncode:  " << strEncode << std::endl;
	std::string strDecode = Encryption::DecodeRSAKeyFile(Encryption::kRSAPriKeyFileName, strEncode);
	std::cout << "RSADecode:  " << strDecode << std::endl << std::endl;
}

void test_rsa_str_key()
{
	std::string strNaked = "I am cswuyg.";
	std::cout << "naked:  " << strNaked << std::endl;
	std::string strEncode = Encryption::EncodeRSAKeyStr(Encryption::kRSAPubKey, strNaked);
	std::cout << "RSAEncode:  " << strEncode << std::endl;
	std::string strDecode = Encryption::DecodeRSAKeyStr(Encryption::kRSAPriKey, strEncode);
	std::cout << "RSADecode:  " << strDecode << std::endl << std::endl;
}

void test_aes()
{
	const int nAESKeyLen = 16;
	unsigned char aesKey[nAESKeyLen + 1] = { 0 };
	RAND_pseudo_bytes((unsigned char*)aesKey, nAESKeyLen);
	std::string strKey((char*)aesKey, 16);
	std::cout << "key:  " << aesKey << std::endl << std::endl;
	std::string strAesKey((char*)aesKey, nAESKeyLen);
	std::string strNaked = "I am cswuyg.....";
	std::cout << "nake:  " << strNaked << std::endl;
	std::string strEncode = Encryption::EncodeAES(strAesKey, strNaked);
	std::cout << "encode:  " << strEncode << std::endl;
	std::string strDecode = Encryption::DecodeAES(strAesKey, strEncode);
	std::cout << "decode  " << strDecode << std::endl << std::endl;
}


int main()
{
	test_rsa_file_key();
	test_rsa_str_key();
	test_aes();
	system("pause");
	return 1;
}
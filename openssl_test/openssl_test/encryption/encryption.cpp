#include "Encryption.h"
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/aes.h>

#pragma comment(lib, "libeay32.lib")

#include <assert.h>


namespace Encryption
{

	const char* kRSAPubKey =	"-----BEGIN PUBLIC KEY-----\n"
		"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDU1QH1n6q23hR9UA+XAnXsNBl4\n"
		"jpt4tCnbqOwNbxvixr4GKQcvfy1/PzZsoB8VfIg23Pf5TAWMXmIT0tMRpptGg3je\n"
		"MExfDm29UDswyHB2pEiqYx7u6HaOUtJyRwBAFm9tJcj3l2LTMaqKAo8jH3pV8i3Q\n"
		"Bo/XZjeBfaghgNnFVwIDAQAB\n"
		"-----END PUBLIC KEY-----\n";

	const char* kRSAPriKey =	"-----BEGIN RSA PRIVATE KEY-----\n"
		"MIICXgIBAAKBgQDU1QH1n6q23hR9UA+XAnXsNBl4jpt4tCnbqOwNbxvixr4GKQcv\n"
		"fy1/PzZsoB8VfIg23Pf5TAWMXmIT0tMRpptGg3jeMExfDm29UDswyHB2pEiqYx7u\n"
		"6HaOUtJyRwBAFm9tJcj3l2LTMaqKAo8jH3pV8i3QBo/XZjeBfaghgNnFVwIDAQAB\n"
		"AoGBAMkzESYlqhoz+cS5YfONJPOGESoyDnHeR8/bckQE7KTG4y8prBxPqzAnDwlD\n"
		"mf8S+iZ7YIUxQ/F97FP3Mwf1q8UU7JkJxb9ESlrl6BxLmTNzB2GeCNbVR7CI0SK7\n"
		"mGbg0mqU6gtVMhVYAh3Qo2WFC8fdGAYi6SDMcGo7Vqpj6l/BAkEA7f3sQTPUsMdu\n"
		"13J3rjFH+D7mQmH3jgfjwkmI0IkkozJCCuJAHAWPuzvKznErVhk5oiw85tPRSI9I\n"
		"DUxjRrp54QJBAOTvuHXDhyWVawsxve8fdNU9ROvsvcbrteeo+WjM0fPpzp7fIRmJ\n"
		"aG1qI/GKeGPg/kLCYwCqJVKe4QsadlTYVjcCQDdJyy3mlZ2v5x8kF4ufJUIVkraP\n"
		"PcyAJrhHDwKrKZjEHzWaFByuahKsNFQ7VD7/QzVao5dYVAFQUAPO4C1ZOuECQQDC\n"
		"kByTMN4didPUSqzhmTlAtHZrjuJmORJ6U/fLIi7YfX8VNAo8WrP+MPNlJspUD/Xm\n"
		"pK9g9DkN82rMdevVBZqXAkEA1kfdVPi0hhLcZlh9PrYU35uH2KY5YO+CJ13BeLXa\n"
		"97lGxc2CzceW3Mia1SljcDoBb5EDZT1w8q3a8prsIKPaew==\n"
		"-----END RSA PRIVATE KEY-----\n";

	const char* kRSAPubKeyFileName = "public.pem";
	const char* kRSAPriKeyFileName = "privkey.pem";

	std::string EncodeRSAKeyFile( const std::string& strPemFileName, const std::string& strData )
	{
		if (strPemFileName.empty() || strData.empty())
		{
			assert(false);
			return "";
		}
		FILE* hPubKeyFile = NULL;
		if(fopen_s(&hPubKeyFile, strPemFileName.c_str(), "rb") || hPubKeyFile == NULL)
		{
			assert(false);
			return ""; 
		}
		std::string strRet;
		RSA* pRSAPublicKey = RSA_new();
		if(PEM_read_RSA_PUBKEY(hPubKeyFile, &pRSAPublicKey, 0, 0) == NULL)
		{
			assert(false);
			return "";
		}

		int nLen = RSA_size(pRSAPublicKey);
		char* pEncode = new char[nLen + 1];
		int ret = RSA_public_encrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pEncode, pRSAPublicKey, RSA_PKCS1_PADDING);
		if (ret >= 0)
		{
			strRet = std::string(pEncode, ret);
		}
		RSA_free(pRSAPublicKey);
		fclose(hPubKeyFile);
		CRYPTO_cleanup_all_ex_data(); 
		return strRet;
	}

	std::string DecodeRSAKeyFile( const std::string& strPemFileName, const std::string& strData )
	{
		if (strPemFileName.empty() || strData.empty())
		{
			assert(false);
			return "";
		}
		FILE* hPriKeyFile = NULL;
		if(fopen_s(&hPriKeyFile, strPemFileName.c_str(),"rb") || hPriKeyFile == NULL)
		{
			assert(false);
			return "";
		}
		std::string strRet;
		RSA* pRSAPriKey = RSA_new();
		if(PEM_read_RSAPrivateKey(hPriKeyFile, &pRSAPriKey, 0, 0) == NULL)
		{
			assert(false);
			return "";
		}
		int nLen = RSA_size(pRSAPriKey);
		char* pDecode = new char[nLen+1];

		int ret = RSA_private_decrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pDecode, pRSAPriKey, RSA_PKCS1_PADDING);
		if(ret >= 0)
		{
			strRet = std::string((char*)pDecode, ret);
		}
		RSA_free(pRSAPriKey);
		fclose(hPriKeyFile);
		CRYPTO_cleanup_all_ex_data(); 
		return strRet;
	}

	std::string EncodeRSAKeyStr( const std::string& strKey, const std::string& strData )
	{
		if (strKey.empty() || strData.empty())
		{
			assert(false);
			return "";
		}
		BIO* pBio = BIO_new_mem_buf((void*)strKey.c_str(), strKey.length());
		RSA* pRSAPubKey = RSA_new();
		if(PEM_read_bio_RSA_PUBKEY(pBio, &pRSAPubKey, NULL, NULL) == NULL)
		{
			assert(false);
			BIO_free(pBio);
			return "";
		}
		BIO_free(pBio);
		std::string strRet;
		int nLen = RSA_size(pRSAPubKey);
		char* pEncode = new char[nLen + 1];
		int ret = RSA_public_encrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pEncode, pRSAPubKey, RSA_PKCS1_PADDING);
		if (ret >= 0)
		{
			strRet = std::string(pEncode, ret);
		}
		RSA_free(pRSAPubKey);
		CRYPTO_cleanup_all_ex_data(); 
		return strRet;
	}

	std::string DecodeRSAKeyStr( const std::string& strKey, const std::string& strData )
	{
		if (strKey.empty() || strData.empty())
		{
			assert(false);
			return "";
		}
		BIO* pBio = BIO_new_mem_buf((void*)strKey.c_str(), (int)strKey.length());
		RSA* pRSAPriKey = PEM_read_bio_RSAPrivateKey(pBio, NULL, NULL, NULL);
		BIO_free(pBio);
		if (pRSAPriKey == NULL)
		{
			assert(false);
			return "";
		}
		std::string strRet;
		int nLen = RSA_size(pRSAPriKey);
		char* pDecode = new char[nLen+1];

		int ret = RSA_private_decrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pDecode, pRSAPriKey, RSA_PKCS1_PADDING);
		if(ret >= 0)
		{
			strRet = std::string((char*)pDecode, ret);
		}
		RSA_free(pRSAPriKey);
		CRYPTO_cleanup_all_ex_data(); 
		return strRet;
	}

	std::string EncodeAES( const std::string& strPassword, const std::string& strData )
	{
		AES_KEY aes_key;
		if(AES_set_encrypt_key((const unsigned char*)strPassword.c_str(), strPassword.length() * 8, &aes_key) < 0)
		{
			assert(false);
			return "";
		}
		std::string strRet;
		for(unsigned int i = 0; i < strData.length()/AES_BLOCK_SIZE; i++)
		{
			std::string str16 = strData.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
			unsigned char out[AES_BLOCK_SIZE];
			AES_encrypt((const unsigned char*)str16.c_str(), out, &aes_key);
			strRet += std::string((const char*)out, AES_BLOCK_SIZE);
		}
		return strRet;
	}

	std::string DecodeAES( const std::string& strPassword, const std::string& strData )
	{
		AES_KEY aes_key;
		if(AES_set_decrypt_key((const unsigned char*)strPassword.c_str(), strPassword.length() * 8, &aes_key) < 0)
		{
			assert(false);
			return "";
		}
		std::string strRet;
		for(unsigned int i = 0; i < strData.length()/AES_BLOCK_SIZE; i++)
		{
			std::string str16 = strData.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
			unsigned char out[AES_BLOCK_SIZE];
			AES_decrypt((const unsigned char*)str16.c_str(), out, &aes_key);
			strRet += std::string((const char*)out, AES_BLOCK_SIZE);
		}
		return strRet;
	}

}
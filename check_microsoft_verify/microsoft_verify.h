/**
microsoft verify file code , only file no catalog.
code base on : http://msdn.microsoft.com/en-us/library/windows/desktop/aa382384(v=vs.85).aspx
cswuyg@gmail.com
2013.12.9
*/
#pragma once

#include <windows.h>
#include <string>

class MicrosoftVerify
{
public:
	MicrosoftVerify(){};
	~MicrosoftVerify(){};


	BOOL VerifyEmbeddedSignature(const std::wstring& file_path);
};
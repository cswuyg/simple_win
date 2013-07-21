/**
An example about how to use _set_invalid_parameter_handler and _set_purecall_handler functioins.
If you want to catch the "invalid memory access" exception, should set the compile command with /EHa.

cswuyg@gmail.com
2013.7.21
*/
#include <windows.h>
#include <string>


void simple_log(const std::wstring& log_msg)
{
	std::wstring strLogWnd = L"cswuyg_simple_debug_log";
	HWND hSend = ::FindWindow(NULL, strLogWnd.c_str());
	COPYDATASTRUCT copydate;
	copydate.cbData = (DWORD)(log_msg.length() + 1) * sizeof(wchar_t);
	copydate.lpData = (PVOID)log_msg.c_str();
	::SendMessage(hSend, WM_COPYDATA, 0, (LPARAM)&copydate);
}

static void MyPureCallHandler(void)
{	
	throw std::invalid_argument("");
	simple_log(L"MyPureCallHandler");
}

static void MyInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
{
	//The parameters all have the value NULL unless a debug version of the CRT library is used.
	throw std::invalid_argument("");
	simple_log(L"MyInvalidParameterHandler");
}

class IPureCall
{
public:
	virtual ~IPureCall(){};
	IPureCall()
	{
		//indirect call the virtual function, the compiler would not treat as "static binding", it is "dynamic binding".
		//At this time, the CPureCall class haven't construct, the virtual table didn't has the pure_call function's point, so it cause "pure virtual function called exception".
		call_by_constructor();
	};
	virtual void pure_call() = 0;
	void call_by_constructor()
	{
		pure_call();
	}
};

class CPureCall : public IPureCall
{
public:
	CPureCall()
	{
	}
	void pure_call()
	{
	}
};


int main()
{
	_set_invalid_parameter_handler(MyInvalidParameterHandler);
	_set_purecall_handler(MyPureCallHandler);
	try
	{
		char* p = NULL;
		printf(p);   // test invalid parameter exception
	}
	catch(...)
	{
		simple_log(L"I catch the invalid parameter exception");
	}
	try
	{
		CPureCall obj; // test _set_purecall_handler
	}
	catch(...)
	{
		simple_log(L"I catch the purecall exception");
	}
	system("pause");
	return 0;
}
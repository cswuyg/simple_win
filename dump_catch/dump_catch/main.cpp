#include "catch_dump_file.h"

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


CatchDumpFile::CDumpCatch g_exception_handler;

void test_release_dump()
{
	//char* p = 0;
	//printf(p);
	CPureCall obj;
}

int main()
{
	test_release_dump();
	system("pause");
	return 1;
}
#include "catch_dump_file.h"

CatchDumpFile::CDumpCatch g_exception_handler;

void test_release_dump()
{
	char* p = 0;
	printf(p);
}

int main()
{
	test_release_dump();
	system("pause");
	return 1;
}
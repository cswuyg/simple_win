// boost_json.cpp : Defines the entry point for the console application.
//
/*
2012-12-10
test boost property_tree for json
*/
#include <myexample/myexample.h>

void TestFunc()
{

	CTest testobj;
	testobj.test_write_json_by_path();
	testobj.test_write_json_by_stream();
	testobj.test_save_by_custom();

	testobj.test_read_json_by_path();
	testobj.test_read_json_by_stream();
	testobj.test_read_json_by_custom();
}


int main(int /*argc*/, char* /*argv[]*/)
{
	TestFunc();
	system("pause");
	return 0;
}


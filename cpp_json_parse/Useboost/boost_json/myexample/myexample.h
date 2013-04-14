#pragma once
/*
2012-12-10
*/
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>
//#include <boost/foreach.hpp>
#include "J:\E\boost/property_tree/ptree.hpp"
#include "J:\E\boost\property_tree/json_parser.hpp"
#include "J:\E\boost/foreach.hpp"
/**
boost_1_52是当前最新版本
来自：http://sourceforge.net/projects/boost/files/boost/
里头有例子，不错，但是json的例子不能写文件。\
能写文件的，value必须是string，不然会编译不过。
如果要自定义key，则需要实现path_of类，不然默认key是string类型,boost已经为string实现了path_of，就在string_path.hpp中。

如果是直接使用read_json依据路径读取，可能会有字符编码的问题，
在使用unicode格式json串的时候，如果直接使用路径读取、写入，使用起来挺乱的，
所以建议先自己读取到stream，再让read_json从stream获取json内容，避开字符编码，
或者更彻底点，完全使用自己的读写文件函数，json库的函数只跟string打交道,在一个工程中，让json库只跟string打交道，这也是一种比较好的分层、封装。
*/

/**
 测试的代码，就不考虑性能了
*/

class CTest
{
public:
	//save
	bool test_write_json_by_path();
	bool test_write_json_by_stream();
	bool test_save_by_custom();

	//read
	boost::property_tree::wptree test_read_json_by_path();
	boost::property_tree::wptree test_read_json_by_stream();
	boost::property_tree::wptree test_read_json_by_custom();
private:
	boost::property_tree::wptree Releaseptree();
	bool ShowJsonInfo(const boost::property_tree::wptree& ptree);
	boost::property_tree::wptree wstrToTree( const std::wstring& strJson );
private:

};
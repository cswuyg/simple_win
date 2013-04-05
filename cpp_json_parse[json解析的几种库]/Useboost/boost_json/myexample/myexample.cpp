#include "myexample.h"
#include <utility/utility.h>

boost::property_tree::wptree CTest::Releaseptree()
{
	using boost::property_tree::wptree;
	wptree ptRoot;
	wptree ptArray;
	wchar_t buf[2] = {L'a', L'\0'};
	for (int i = 0; i != 2; ++i)
	{
		ptArray.push_back(std::make_pair(L"", buf));
		buf[0] += 1;
	}
	boost::property_tree::wptree ptSecond;
	ptSecond.put(L"x阿x", L"y哈哈y");
	ptArray.push_back(std::make_pair(L"", ptSecond));
	ptRoot.add_child(L"root", ptArray);
	ptRoot.add(L"first", L"cswuyg");
	return ptRoot;

	/*
	{
		"root":
		[
			"a",
			"b", 
			{"x阿x": "y哈哈y"}
		],
		"first": "cswuyg"
	}
	*/
}

bool CTest::test_write_json_by_path()
{
	boost::property_tree::wptree ptRoot = Releaseptree();
	try
	{
		boost::property_tree::json_parser::write_json("c:\\testsavejson.txt", ptRoot ); //路径是窄字节，ptree是宽字节，看着不舒服
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool CTest::test_write_json_by_stream()
{
	bool bRet = true;
	boost::property_tree::wptree ptRoot = Releaseptree();
	std::wofstream wofs(L"c:\\testsavejson2.txt");
	do 
	{
		try
		{
			write_json(wofs, ptRoot);
		}
		catch (std::exception& )
		{
			bRet = false;
			break;
		}
	} while (false);
	wofs.close();
	return bRet;
}

bool CTest::test_save_by_custom()
{
	boost::property_tree::wptree ptRoot = Releaseptree();
	std::wstring strJson;
	std::wstringstream wstrstream;
	write_json(wstrstream, ptRoot);
	strJson = wstrstream.str();
	std::string strAnsi = wstr2str(strJson, CP_ACP);

	WriteToDiskA(L"c:\\testsavejsonbycustom.txt", strAnsi);
	return true;
}


boost::property_tree::wptree CTest::test_read_json_by_path()
{
	boost::property_tree::wptree ptree;
	boost::property_tree::json_parser::read_json("c:\\testsavejson.txt", ptree);
	ShowJsonInfo(ptree);
	return ptree;
}

bool CTest::ShowJsonInfo( const boost::property_tree::wptree& ptree )
{
	boost::property_tree::wptree ptreeTemp = ptree;
	try
	{	
		BOOST_FOREACH( boost::property_tree::wptree::value_type& vInputChild, ptreeTemp )
		{
			if (!vInputChild.first.empty())
			{
				std::wstring strFirst = vInputChild.first;
				std::cout << wstr2str(strFirst, CP_ACP) << ":";
			}
			boost::property_tree::wptree& ptSecond = vInputChild.second;
			if(ptSecond.empty())
			{
				std::wstring strSecond = ptSecond.get_value<std::wstring>();
				std::cout << wstr2str(strSecond, CP_ACP) << std::endl;
			}
			else
			{
				std::cout << std::endl;
				ShowJsonInfo(ptSecond);
			}
		}
	}
	catch ( std::exception& )
	{
		return false;
	}
	return true;
}

boost::property_tree::wptree CTest::test_read_json_by_stream()
{
	boost::property_tree::wptree ptRoot;
	std::wifstream wifs(L"c:\\testsavejson2.txt");
	do 
	{
		try
		{
			read_json(wifs, ptRoot);
		}
		catch (...)
		{
			break;
		}
	} while (false);
	wifs.close();
	ShowJsonInfo(ptRoot);
	return ptRoot;
}

boost::property_tree::wptree CTest::test_read_json_by_custom()
{
	boost::property_tree::wptree ptRoot;
	std::string strData;
	ReadFromDiskA(L"c:\\testsavejsonbycustom.txt", strData);
	std::wstring strDataCopy = str2wstr(strData, CP_ACP);
	boost::property_tree::wptree ptree = wstrToTree(strDataCopy);
	ShowJsonInfo(ptree);
	return ptree;
}

boost::property_tree::wptree CTest::wstrToTree( const std::wstring& strJson )
{
	boost::property_tree::wptree ptInputTree;
	std::wstringstream strstream;
	strstream << strJson;
	std::wstring ssx = strstream.str();
	read_json( strstream, ptInputTree );
	return ptInputTree;
}
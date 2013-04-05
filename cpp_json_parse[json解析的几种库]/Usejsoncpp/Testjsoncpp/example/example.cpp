#include "example.h"

bool CTestjsoncpp::test_Parse()
{
	std::wstring wstrJson = ReleaseJsonStr();
	std::string strJson = wstr2str(wstrJson, CP_UTF8);
	Json::Reader jsonReader;
	Json::Value root;
	jsonReader.parse(strJson, root);

	std::string strresult = root["result"].asString();
	std::cout << "result:" << strresult << std::endl;

	std::cout << "data:" << std::endl;
	Json::Value jvData = root["data"];
	if(jvData.isArray())
	{
		std::cout << "[" << std::endl;
		//item
		for (Json::Value::iterator it = jvData.begin(); it != jvData.end(); ++it)
		{
			std::cout << "{" << std::endl;
			Json::Value item = *it;
			Json::Value::Members memItem = item.getMemberNames();
			//mem in item
			for (Json::Value::Members::iterator itItem = memItem.begin(); itItem != memItem.end(); ++itItem)
			{
				std::string strKey = *itItem;
				std::string strValue = item[strKey].asString();
				std::cout << strKey << ":" << strValue << std::endl;
			}
			std::cout << "}" << std::endl;
		}
		std::cout << "]" << std::endl;
	}
	return false;
}

Json::Value CTestjsoncpp::ReleaseJsonValue()
{
	Json::Value jvRoot;
	jvRoot["result"] = "ok";  //对象元素
	Json::Value jvData;
	Json::Value jvItem1;
	jvItem1["name"] = "cswuyg_1";
	jvItem1["age"] = "2012_1";
	jvData.append(jvItem1);      //数组元素
	Json::Value jvItem2;
	jvItem2["没意思"] = "呵呵";
	jvItem2["age"] = "2012_2";
	jvData.append(jvItem2);
	jvRoot["data"] = jvData;
	return jvRoot;
}

std::wstring CTestjsoncpp::ReleaseJsonStr()
{
	Json::Value jvData = ReleaseJsonValue();

	//压缩输出
	//Json::FastWriter fastwriter;
	//std::string strJson = fastwriter.write(jvData);
	//return str2wstr(strJson, CP_ACP);

	//格式化输出
	Json::StyledWriter stylewriter;
	std::string strJson = stylewriter.write(jvData);
	return str2wstr(strJson, CP_UTF8);
}

bool CTestjsoncpp::test_writefile()
{
	Json::Value jvData = ReleaseJsonValue();

	Json::StyledWriter stylewriter;
	std::string strStyleJson = stylewriter.write(jvData);
	WriteToDiskA(L"c:\\jsoncppstylewrite", strStyleJson);

	//压缩输出
	Json::FastWriter fastwriter;
	std::string strFastJson = fastwriter.write(jvData);
	WriteToDiskA(L"c:\\jsoncppfastwrite", strFastJson);
	return true;
}

bool CTestjsoncpp::test_readfile()
{
	std::string strFastJson;
	ReadFromDiskA(L"c:\\jsoncppfastwrite", strFastJson);
	Json::Reader jsonReader;
	Json::Value root;
	jsonReader.parse(strFastJson, root);
	std::string strresult = root["result"].asString();
	std::cout << strresult << std::endl;
	//...

	return true;
}

/*
{
"data" : [
{
"age" : "2012_1",
"name" : "cswuyg_1"
},
{
"age" : "2012_2",
"没意思" : "呵呵"
}
],
"result" : "ok"
}

*/
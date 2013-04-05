#include "test.h"

/*
{
"contents": [{
"Test": "Test,Obj"
},
"HelloWorld,cswuyg",
"Good Good Studey,Day Day Up"],
"result": "no_way"
}
*/
std::wstring CTestJSON::GetJsonStr()
{
	JSONArray jsArray;
	JSONObject jsObj;
	jsObj[L"Test"] = new(std::nothrow)JSONValue(L"Test,Obj");
	jsArray.push_back(new(std::nothrow)JSONValue(jsObj));
	jsArray.push_back(new(std::nothrow)JSONValue(L"HelloWorld,cswuyg"));
	jsArray.push_back(new(std::nothrow)JSONValue(L"Good Good Studey,Day Day Up"));
	jsArray.push_back(new(std::nothrow)JSONValue((double)1988));

	JSONObject jsObjNew;
	jsObjNew[L"contents"] = new(std::nothrow)JSONValue(jsArray);
	jsObjNew[L"result"] = new(std::nothrow)JSONValue(L"no_way");

	JSONValue jsValue = jsObjNew;
	std::wstring strRet = jsValue.Stringify().c_str();
	return strRet;
}

void CTestJSON::parseJsonStr( const std::wstring& strJsonStr )
{
	JSONValue* jsInput = JSON::Parse(strJsonStr.c_str());
	if (jsInput == NULL || !jsInput->IsObject())
	{
		return;
	}

	JSONObject::const_iterator itResult = jsInput->AsObject().find(L"result");
	if (itResult != jsInput->AsObject().end())
	{
		std::wstring strResult = itResult->second->AsString();
		std::wcout << L"result" << L":" << strResult << std::endl;
	}

	JSONObject::const_iterator itLove = jsInput->AsObject().find(L"Love");
	if (itLove != jsInput->AsObject().end())
	{
		std::wstring strResult = itLove->second->AsString();
		std::wcout << L"Love" << L":" << strResult << std::endl;
	}

	JSONArray jsArray;
	JSONObject::const_iterator itContents = jsInput->AsObject().find(L"contents");
	if (itContents != jsInput->AsObject().end() && itContents->second != NULL && itContents->second->IsArray())
	{
		jsArray = itContents->second->AsArray();
	}

	std::wcout << "[" << std::endl;
	JSONArray::iterator it = jsArray.begin();
	JSONArray::iterator itEnd = jsArray.end();
	for (; it != itEnd; ++it)
	{
		JSONValue* jsValue = *it;
		if (jsValue->IsObject())
		{
			jsValue->AsObject();
			JSONObject::const_iterator itObj = jsValue->AsObject().begin();
			JSONObject::const_iterator itObjEnd = jsValue->AsObject().end();
			for (; itObj != itObjEnd; ++itObj)
			{
				std::wstring strValue = itObj->second->AsString();
				std::wcout << L"{" << itObj->first << L":" << strValue << L"}" << std::endl;
			}
		}
		else if (jsValue->IsString())
		{		
			std::wstring strValue = jsValue->AsString();
			std::wcout << strValue << std::endl;
		}
		else if (jsValue->IsNumber())
		{
			double dValue = jsValue->AsNumber();
			std::wcout << dValue << std::endl;
		}
		//...
	}
	std::wcout << "]" << std::endl;
}

std::wstring CTestJSON::AddJsonStr( const std::wstring& strJsonStr )
{
	/*
	SimpleJson 库插入是非常麻烦的事情。
	JSONValue对象有智能指针的功能，会给你析构掉它所包含的JSON对象
	而JSON的as....函数，返回的是const类型的引用，如果是array类型，那么是JSONValue*的浅拷贝
	对Parse的返回值实行delete之后，JSONValue又会再delete一次，于是出现多次析构的错误
	所以必须保证，要么只有JSONValue对象去执行析构，要么只有主动的delete Parse的返回值。
	对于插入来说，这种逻辑会带来麻烦。定义了一个JSONValue，浅拷贝了parse返回值的一部分json对象，
	然后JSONValue析构了浅拷贝的JSONValue*，先对Parse的返回值则很难做处理，如果delete，则多析构了JSON对象，
	如果不delete，则Parse内部new的map内存没有被析构。

	解决办法有两种：
	1、不要定义JSONValue对象，而是定义JSONValue引用，因为我要往JSONValue里插值，所以必须用到const_cast。
	2、递归拷贝出JSONValue*里的字符串格式的JSON对象，然后再Parse之后进入新JSONObj，
	保证新、旧对象分离。
	*/
	JSONValue* jsInput = JSON::Parse(strJsonStr.c_str());
	if (jsInput == NULL || !jsInput->IsObject())
	{
		return L"";
	}

	std::wstring strRet;
	JSONObject jsObjNew;
	JSONObject::const_iterator it = jsInput->AsObject().begin();
	JSONObject::const_iterator itEnd = jsInput->AsObject().end();
	for (; it != itEnd; ++it)
	{
		std::wstring strFirst = it->first.c_str();
		std::wstring strSecond = it->second->Stringify().c_str();
		JSONValue* pTemp = JSON::Parse(strSecond.c_str());
		jsObjNew[strFirst] = pTemp;
	}
	jsObjNew[L"Love"] = new(std::nothrow)JSONValue(L"is Happiness");
	JSONValue jsValueNew = jsObjNew;
	strRet = jsValueNew.Stringify();
	return strRet;
}
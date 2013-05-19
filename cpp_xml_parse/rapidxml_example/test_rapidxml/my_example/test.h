#pragma once 
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>

const wchar_t* const const_xml_path = L"c:\\test_rapid_xml.xml";

/*
测试使用rapidxml
2012-12-14
*/
class CTest
{
public:
	void test_load_xml();
	void test_release_xml();

private:
	/**
	需要做utf8的转码
	*/
	void AddXMLAttributeValue(rapidxml::xml_document<>& pDoc, rapidxml::xml_node<>* pNode, const std::wstring& strKey, const std::wstring& strValue);
	std::wstring GetXMLAttributeValue(rapidxml::xml_node<>* pNode, const std::wstring& strwKey);
};
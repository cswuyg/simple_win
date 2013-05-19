#include "test.h"
#include <iostream>
#include <string>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>
#include "utility.h"

void CTest::test_load_xml()
{
	//file<> fdoc( const_xml_path.c_str() );  //不直接用库提供的读文件函数
	std::string straData;
	ReadFromDiskA(const_xml_path, straData);
	rapidxml::xml_document<> xmlDoc;
	xmlDoc.parse<0>(const_cast<char*>(straData.c_str()));
	rapidxml::xml_node<>* xmlRoot = xmlDoc.first_node("root");
	if (xmlRoot != NULL)
	{
		for ( rapidxml::xml_node<>* itemNode = xmlRoot->first_node("item"); itemNode != NULL; itemNode = itemNode->next_sibling() )
		{
			std::wstring strwValueID = GetXMLAttributeValue(itemNode, L"id");
			std::wstring strwValueTime = GetXMLAttributeValue(itemNode, L"time");
			std::wstring strwValueLove = GetXMLAttributeValue(itemNode, L"love");
			std::wstring strwValueData = GetXMLAttributeValue(itemNode, L"data");
			std::wstring strwValuehei = GetXMLAttributeValue(itemNode, L"嘿");

			std::cout << "id = " << wstr2str(strwValueID, CP_ACP) << std::endl;
			std::cout << "time = " << wstr2str(strwValueTime, CP_ACP) << std::endl;
			std::cout << "love = " << wstr2str(strwValueLove, CP_ACP) << std::endl;
			std::cout << "data = " << wstr2str(strwValueData, CP_ACP) << std::endl;
			std::cout << "hei = " << wstr2str(strwValuehei, CP_ACP) << std::endl;

			rapidxml::xml_node<>* xmlChild = itemNode->first_node("child_item");
			if (xmlChild != NULL)
			{
				std::string strValueChildItem = xmlChild->value();
				std::cout << "child_item = " << strValueChildItem << std::endl;
			}
		}
	}
}

void CTest::test_release_xml()
{
	rapidxml::xml_document<> xmlDoc;
	rapidxml::xml_node<>* xmlDeclaration = xmlDoc.allocate_node(rapidxml::node_pi, xmlDoc.allocate_string("xml version='1.0' encoding='utf-8'"));
	xmlDoc.append_node(xmlDeclaration);
	//根节点
	rapidxml::xml_node<>* xml_root_node = xmlDoc.allocate_node(rapidxml::node_element, "root", NULL);
	//子节点
	rapidxml::xml_node<>* xmlNode = xmlDoc.allocate_node(rapidxml::node_element, "item");

	std::wstring strKeyID = L"id";
	std::wstring strValueID = L"cswuyg";
	std::wstring strKeyTime = L"time";
	std::wstring strValueTime = L"1988";
	std::wstring strKeyLove = L"love";
	std::wstring strValueLove = L"freedom";
	std::wstring strKeyData = L"data";
	std::wstring strValueData = L"测试测试，111111";
	std::wstring strKeyhei = L"嘿";
	std::wstring strValuehei = L"嘿";

	AddXMLAttributeValue(xmlDoc, xmlNode, strKeyID, strValueID);
	AddXMLAttributeValue(xmlDoc, xmlNode, strKeyTime, strValueTime);
	AddXMLAttributeValue(xmlDoc, xmlNode, strKeyLove, strValueLove);
	AddXMLAttributeValue(xmlDoc, xmlNode, strKeyData, strValueData);
	AddXMLAttributeValue(xmlDoc, xmlNode, strKeyhei, strValuehei);

	//子节点的子节点
	rapidxml::xml_node<>* childNode = xmlDoc.allocate_node(rapidxml::node_element, xmlDoc.allocate_string("child_item"));
	childNode->value( "hello" );
	xmlNode->append_node(childNode);
	xml_root_node->append_node( xmlNode);
	//加到doc中
	xmlDoc.append_node(xml_root_node);
	std::string text;  
	rapidxml::print(std::back_inserter(text), xmlDoc, 0);

	WriteToDiskA(const_xml_path, text);
}

void CTest::AddXMLAttributeValue( rapidxml::xml_document<>& pDoc, rapidxml::xml_node<>* pNode, const std::wstring& strwKey, const std::wstring& strwValue )
{
	std::string straValue = wstr2str(strwValue, CP_UTF8);
	std::string strakey = wstr2str(strwKey, CP_UTF8);
	pNode->append_attribute(pDoc.allocate_attribute(pDoc.allocate_string(strakey.c_str()), pDoc.allocate_string(straValue.c_str()) ) );
}

std::wstring CTest::GetXMLAttributeValue( rapidxml::xml_node<>* pNode, const std::wstring& strwKey )
{
	std::string straKey = wstr2str(strwKey, CP_UTF8);
	std::wstring strwValue;
	rapidxml::xml_attribute<>* xmlattribute = pNode->first_attribute(straKey.c_str()); 
	if (xmlattribute != NULL)
	{
		std::string straValue = xmlattribute->value();
		strwValue = str2wstr(straValue, CP_UTF8);
	}
	return strwValue;
}
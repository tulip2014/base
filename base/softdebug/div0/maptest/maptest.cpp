// maptest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <string>

typedef struct _NODE
{
	int a;
	int b;
}NODE;

class CNode
{
public:
	std::map<std::wstring, std::wstring> test;
};

int _tmain(int argc, _TCHAR* argv[])
{

	std::map<std::wstring, std::wstring> test;
	test[L"a"] = L"a1";
	test[L"a"] = L"a2";
	test[L"b"] = L"b2";
	test[L"c"] = L"c2";
	test[L"d"] = L"d2";
	test[L"y"];

	test.insert(std::pair<std::wstring, std::wstring>(L"a", L"e3"));
	test.insert(std::pair<std::wstring, std::wstring>(L"e", L"e2"));
	test.insert(std::pair<std::wstring, std::wstring>(L"e", L"e3"));
	test.insert(std::pair<std::wstring, std::wstring>(L"f", L"f2"));
	test.insert(std::pair<std::wstring, std::wstring>(L"g", L"g2"));

	std::wstring wsTmp = test[L""];
	test.clear();
	 = NULL;
	std::map<std::wstring, std::wstring>::iterator iter = test.find(L"");

	std::map<std::wstring, std::wstring>::iterator it = test.begin();
	while(it != test.end())
	{
		std::pair<std::wstring, std::wstring> p = (*it);
		std::wcout<<L"key:"<<p.first.c_str()<<L", value:"<< p.second.c_str() <<std::endl;
		it++;
	}

	return 0;
}


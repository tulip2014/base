// TestDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "TestDll.h"



// 这是导出变量的一个示例
TESTDLL_API int nTestDll = 2;

// 这是导出函数的一个示例。
TESTDLL_API int fnTestDll(int a , int b)
{
	return a + b;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 TestDll.h
CTestDll::CTestDll()
{
	return;
}


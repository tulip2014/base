// div0.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>

using namespace std;

int iBase = 5;
int iDiv = 0;
int iResult = 0;

int HandleException(long lExceptionCode)
{
	if (lExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO)
	{
		iDiv = 5;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	__try
	{
		iResult = iBase / iDiv;
		cout<<"iresult="<<iResult<<endl;
	}
	__except (HandleException(GetExceptionCode()))
	{
		cout<<"exception"<<endl;
	}

	system("pause");
	return 0;
}


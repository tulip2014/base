// second.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "second.h"

int add(int d, int e)
{
	int f = 0;
	f = d + e;
	return f;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	int a = 1;
	int b = 2;
	int c = add(4, 5);
	
	return 0;
}


// second.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "second.h"

int add(int a, int b)
{
	int c = 0;
	a = a + b;
	return c;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	int a = 1;
	int b = 2;
	int c = add(a, b);
	
	return 0;
}


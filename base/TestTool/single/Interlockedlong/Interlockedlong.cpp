// Interlockedlong.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Interlockedlong.h"
#include "ThreadTest.h"

DWORD *dwSize = NULL;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。

	dwSize = (DWORD*) _aligned_malloc(4,4);
	*dwSize = 0;

	ThreadTest a(1), b(2), c(3);
	a.SetData(dwSize);
	b.SetData(dwSize);
	c.SetData(dwSize);

	a.Start();
	b.Start();
	c.Start();

	InterlockedPopEntrySList();

	Sleep(200000);
	return 0;
}

// TestMain.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestMain.h"
#include "baseoperator.h"
#include "yBase/baselib/com/IUnknown.h"

typedef HRESULT (__stdcall* MyDllCreateObject)(void **ppv );
typedef HRESULT (__stdcall* IUDllCreateObject)(void **ppv );

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	HMODULE hWnd = LoadLibraryW(L"IUnknowd.dll");
	if (hWnd == NULL)
	{
		return 1;
	}

	IUDllCreateObject test = (IUDllCreateObject)GetProcAddress(hWnd, "DllCreateObject");

	if (NULL == test)
	{
		return 2;
	}

	IUnknow* pI = NULL;
	test((void**)&pI);

	IX* pX = NULL;
	HRESULT hr = pI->QueryInterface(2, (void**)&pX);
	if (SUCCEEDED(hr))
	{
		pX->PrintX();
		pX->Release();
	}

	pI->Release();

	return 0;
}


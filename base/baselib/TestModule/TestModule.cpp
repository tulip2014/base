// TestModule.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestModule.h"
#include "shlwapi.h"

#pragma comment(lib, "shlwapi.lib")

typedef int (__cdecl* lpFunc)();


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	PathAppend(szPath, _T("TestDll.dll"));


 	HMODULE hDll = LoadLibrary(szPath);
	if (NULL == hDll) {
		return ;
	}

	lpFunc pfunc=(lpFunc)::GetProcAddress(hDll, "run");



	return 0;
}


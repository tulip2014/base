// TestModule.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TestModule.h"
#include "shlwapi.h"
//#include "TestDll.h"

#pragma comment(lib, "shlwapi.lib")
//#pragma comment(lib, "TestDlld.lib")

typedef int (* lpFunc)(int, int);
void DynamicLoad() {
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	PathAppend(szPath, _T("TestDlld.dll"));


	HMODULE hDll = LoadLibrary(szPath);
	if (NULL == hDll) {
		return ;
	}

	lpFunc pfunc = (lpFunc)::GetProcAddress(hDll, "fnTestDll");
	int a = 1;
	int b = 2;
	int c = pfunc(a, b);

	FreeLibrary(hDll);
}

void GetStaticVariable() {
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	PathRemoveFileSpec(szPath);
	PathAppend(szPath, _T("TestDlld.dll"));

	HMODULE hTest = LoadLibrary(szPath);
	if (NULL == hTest)
	{
		return;
	}

	int a = 0;
	a = *(int*)GetProcAddress(hTest, "nTestDll");
	
	TCHAR szResult[MAX_PATH] = {0};
	_stprintf_s(szResult, MAX_PATH, _T("%d"), a);
	MessageBox(NULL, szResult, _T("test"), MB_OK);
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//DynamicLoad();

	//GetStaticVariable();
	Sleep(10000000);
	//StaticLoad();
	return 0;
}


// basehelper.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "basehelper.h"
#include "utils/utils.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	RegUtils::DeleteRegValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\V9\\WinZipper\\General", L"test");


 	return 0;
}


// basehelper.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "basehelper.h"
#include "utils/OSHelper.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	bool a = OSHelper::IsWow64();

	TCHAR szUserName[MAX_PATH] = {0};
	DWORD dwSize = MAX_PATH;
	GetUserName(szUserName, &dwSize);

 	return 0;
}


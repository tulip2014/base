// yBase.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "baselib.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	TCHAR szLogicDriveStrings[MAX_PATH];

	ZeroMemory(szLogicDriveStrings, MAX_PATH);

	DWORD dwLen = GetLogicalDriveStringsW(MAX_PATH - 1, szLogicDriveStrings);
	DWORD dwNum = dwLen / sizeof(TCHAR);
	UINT uDriverType = GetDriveTypeW(szLogicDriveStrings);
	//DRIVE_UNKNOWN

	std::vector<std::wstring> vecProcess;
	base::proc::EnumProcess(vecProcess);

	//system("pause");
	return 0;
}


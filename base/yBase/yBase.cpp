// yBase.cpp : �������̨Ӧ�ó������ڵ㡣
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

	// TODO: �ڴ˷��ô��롣
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


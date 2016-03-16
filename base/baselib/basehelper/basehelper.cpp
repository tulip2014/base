// basehelper.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "basehelper.h"
#include "utils/utils.h"
#include "process/ProcessUtils.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CCmdLine cmd(lpCmdLine);
	std::wstring wsPath = cmd[L"path"];

	bool bAdmin = false;
	if (cmd.HasParam(L"admin"))
	{
		bAdmin = true;
	}

	ProcUtils::CreateProcess(wsPath, L"", L"", bAdmin, false, true);

 	return 0;
}


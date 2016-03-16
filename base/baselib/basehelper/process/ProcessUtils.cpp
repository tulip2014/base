
#include "ProcessUtils.h"
#include "OSUtils.h"
#include <Psapi.h>
#include <shellapi.h>

namespace ProcUtils
{
	bool CreateProcess(const std::wstring& sProcessName, const std::wstring& sCmdLine, const std::wstring& sCurrentDirectory, bool bForceAdmin, bool bWaitProcess,bool bIsshow)
	{
		BOOL bRet = FALSE;
		HANDLE hProcess = NULL;
		TCHAR szCmd[MAX_PATH * 2] = {0};

		SHELLEXECUTEINFO sei = {sizeof(SHELLEXECUTEINFO)};

		sei.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI;
		sei.hwnd = NULL;
		if (OSUtils::IsVistaOrLater())
			sei.lpVerb = bForceAdmin ? L"runas" : L"open";
		else
			sei.lpVerb = L"open";

		sei.lpFile = sProcessName.c_str();
		sei.lpParameters = sCmdLine.c_str();
		bIsshow ? sei.nShow = SW_SHOWNORMAL : sei.nShow = SW_HIDE;
		sei.lpDirectory = sCurrentDirectory.c_str();

		bRet = ::ShellExecuteEx(&sei);

		hProcess = sei.hProcess;

		if (bRet)
		{
			if (bWaitProcess)
			{
				::WaitForSingleObject(hProcess, INFINITE);
			}
			::CloseHandle(hProcess);
		}

		return bRet ? true : false;
	}
}
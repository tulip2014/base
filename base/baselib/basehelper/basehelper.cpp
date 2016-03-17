// basehelper.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "basehelper.h"
#include "utils/utils.h"
#include "process/ProcessUtils.h"
#include "safe/cryptopp/md5Utils.h"
#include "safe/cryptopp/base64Utils.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::wstring a = safe::crypt::Base64EncodeW(L"asdf");

 	return 0;
}


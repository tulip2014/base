// T2Uncode.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "T2Uncode.h"


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣

	TCHAR szName[10] = {0};
	_tcscat_s(szName, 10, _T("1234567890"));
	
	return 0;
}


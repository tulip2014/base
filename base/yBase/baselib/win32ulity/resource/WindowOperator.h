
#pragma once
#include "windows.h"
#include "tchar.h"

//Ϊ����������������ǰ
typedef void (WINAPI *PSWITCHTOTHISWINDOW) (HWND,BOOL);

bool SetWindowToFront()
{
	HWND hwnd = ::FindWindow(_T("classname"), NULL);
	if (NULL == hwnd)
	{
		return false;
	}

	// 	PSWITCHTOTHISWINDOW SwitchToThisWindow;
	// 	HMODULE hUser32=GetModuleHandle(_T("user32"));
	// 	SwitchToThisWindow=(PSWITCHTOTHISWINDOW)GetProcAddress(hUser32,"SwitchToThisWindow");
	// 	SwitchToThisWindow(hwnd,TRUE);

	int iRet = SetForegroundWindow(hwnd) ;

	return iRet;
}

#include "stdafx.h"
#include "iBaseOpreator.h"

LPCTSTR IsStringEmpty(std::wstring& strValue)
{
	if (!strValue.empty())
	{
		return strValue.c_str();
	}
	return NULL;
}

void CBaseOpreator::AddRef()
{

}

void CBaseOpreator::Release()
{
	delete this;
}

int CBaseOpreator::Add(int a, int b)
{
	return a + b;
}

long CBaseOpreator::MouseMove(POINT point)
{
	SetCursorPos(point.x, point.y);
	return 0;
}

long CBaseOpreator::MouseClick(LPCTSTR lpType, int iTimes)
{
	std::wstring strType = lpType;
	if (_T("L") == strType && 1 == iTimes)  //左键单击
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		Sleep(8);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	}
	else if (_T("L") == strType && 2 == iTimes)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
		Sleep(9);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	}
	else if (_T("R") == strType && 1 == iTimes)
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	}
	else if (_T("R") == strType && 2 == iTimes)
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
		Sleep(9);
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	}
	else if (_T("M") == strType && 1 == iTimes)
	{
		mouse_event(MOUSEEVENTF_MIDDLEDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_MIDDLEUP,0,0,0,0);
	}
	else if (_T("M") == strType && 2 == iTimes)
	{
	}
	else
	{
		return 1;  //参数不规范
	}

	return 0;
}

long CBaseOpreator::GetMousePos(POINT& point)
{
	BOOL bRet = ::GetCursorPos(&point);

	return !bRet;
}

long CBaseOpreator::KeyboardDown(int iType)
{
	keybd_event(iType, 0, 0, 0);
	return 0;
}

long CBaseOpreator::KeyboardUp(int iTypet)
{
	keybd_event(iTypet, 0, KEYEVENTF_KEYUP, 0);
	return 0;
}

long CBaseOpreator::SetWindowFocus(LPCTSTR lpcClassName, LPCTSTR lpcTitleName)
{
	std::wstring strClassName = lpcClassName;
	std::wstring strTitleName = lpcTitleName;

	LPCTSTR lpClassName = IsStringEmpty(strClassName);
	LPCTSTR lpTitleName = IsStringEmpty(strTitleName);


	HWND hwnd = ::FindWindow(lpClassName, lpTitleName);
	if (NULL == hwnd)
	{
		return 1;
	}

	::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	return 0;
}

long CBaseOpreator::ShowWindows(LPCTSTR lpcClassName, LPCTSTR lpcTitleName, int iType)
{
	std::wstring strClassName = lpcClassName;
	std::wstring strTitleName = lpcTitleName;

	LPCTSTR lpClassName = IsStringEmpty(strClassName);
	LPCTSTR lpTitleName = IsStringEmpty(strTitleName);

	HWND hwnd = ::FindWindow(lpClassName, lpTitleName);
	if (NULL == hwnd)
	{
		return 1;
	}

	ShowWindow(hwnd, iType);   //正常显示

	return 0;
}

long CBaseOpreator::GetWindowPos(LPCTSTR lpcClassName, LPCTSTR lpcTitleName, RECT& rect)
{
	std::wstring strClassName = lpcClassName;
	std::wstring strTitleName = lpcTitleName;

	LPCTSTR lpClassName = IsStringEmpty(strClassName);
	LPCTSTR lpTitleName = IsStringEmpty(strTitleName);

	HWND hwnd = ::FindWindow(lpClassName, lpTitleName);
	if (NULL == hwnd)
	{
		return 1;
	}

	rect.left = rect.top = rect.right = rect.bottom = 0;
	BOOL bRet = GetWindowRect(hwnd, &rect);
	if (bRet)  //get success
	{
		return 0;
	}

	return 1;
}

long CBaseOpreator::IsWindowExit(LPCTSTR lpcClassName, LPCTSTR lpcTitleName)
{
	std::wstring strClassName = lpcClassName;
	std::wstring strTitleName = lpcTitleName;

	LPCTSTR lpClassName = IsStringEmpty(strClassName);
	LPCTSTR lpTitleName = IsStringEmpty(strTitleName);

	HWND hwnd = ::FindWindow(lpClassName, lpTitleName);
	if (NULL == hwnd)
	{
		return 1;
	}

	return 0;
}

long CBaseOpreator::GetPiexlColor(POINT pos, int& r, int& g, int& b)
{
	HDC hDC = ::GetDC(NULL);
	COLORREF colorref = ::GetPixel(hDC, pos.x, pos.y);  //Get the cursor color
	::ReleaseDC(NULL,hDC);

	r = (colorref&0xFF);
	g = ((colorref>>8)&0xFF);
	b = (colorref>>16);

	return 0;
}
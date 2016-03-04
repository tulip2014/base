
#pragma once
#include "windows.h"
#include "tchar.h"

//use to DynamicLoad
class GetFunc
{
private:
	HINSTANCE m_hIns;
public:
	GetFunc()
	{
		m_hIns = NULL;
	}
	~GetFunc()
	{
		if (NULL != m_hIns)
		{
			FreeLibrary(m_hIns);
			m_hIns = NULL;
		}
	}
	LPVOID GetFuncName(LPCTSTR lpDllName, LPCSTR lpFuncName)
	{
		m_hIns = LoadLibrary(lpDllName);
		if (NULL == m_hIns)
		{
			return NULL;
		}
		LPVOID lpFun = (LPVOID)GetProcAddress(m_hIns, lpFuncName);
		return lpFun;
	}
};

#define  DynamicRun(lptDllName,lpFuncName, lpDynimicFunc, iRet)   LPVOID lpDynimicFunc;  \
	GetFunc getfunc;		\
	lpDynimicFunc = getfunc.GetFuncName((lptDllName), (lpFuncName));   \
	if (NULL == lpDynimicFunc)												  \
	return iRet;														  \


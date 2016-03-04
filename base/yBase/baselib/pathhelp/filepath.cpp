
//#include "stdafx.h"
#include "filepath.h"


BOOL CreateDirectory(LPCTSTR lpszPath)
{
	TCHAR tcPath[MAX_PATHEX];
	_tcscpy_s(tcPath,MAX_PATHEX,lpszPath);

	for(DWORD i=0;i<_tcslen(tcPath);i++)
	{
		if(tcPath[i]==_T('/'))
			tcPath[i]=_T('\\');
	}

	TCHAR *p=tcPath+3;

	while(TRUE)
	{
		p=_tcsstr(p,_T("\\"));

		if(p==NULL)
		{
			break;
		}
		else
		{
			*p=_T('\0');
			::CreateDirectory(tcPath,NULL);
			*p=_T('\\');
			p++;
		}
	}

	return TRUE;
}
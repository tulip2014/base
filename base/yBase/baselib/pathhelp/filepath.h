
#pragma once
#include "tchar.h"
#include "windows.h"


#define		MAX_PATHEX		MAX_PATH*2

namespace base 
{
	namespace filepath
	{
		BOOL CreateDirectory(LPCTSTR lpszPath);
		BOOL CopyFile(LPCTSTR lpszSrcPath,LPCTSTR lpszDesPath,BOOL bFailIfExists);
	}
}
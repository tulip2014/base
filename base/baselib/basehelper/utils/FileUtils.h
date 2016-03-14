/********************************************************************
	created:	2016/03/14
	created:	14:3:2016   17:01
	filename: 	base\baselib\basehelper\utils\FileUtils.h
	file path:	base\baselib\basehelper\utils
	file base:	FileUtils
	file ext:	h
	author:		murisly
	
	purpose:	base file operator
*********************************************************************/

#pragma once


#include <windows.h>

namespace FileUtils {
	BOOL WriteDataFileW(PVOID pBuf, DWORD dwLength, const wchar_t* const ptcPath);
	BOOL ReadDataFileW(PVOID *ppBuf, DWORD *pLength, const wchar_t* const ptcPath, DWORD dwExtendLength);

	BOOL WriteDataFileA(PVOID pBuf, DWORD dwLength, const char* const ptcPath);
	BOOL ReadDataFileA(PVOID *ppBuf, DWORD *pLength, const char* const ptcPath, DWORD dwExtendLength);
}

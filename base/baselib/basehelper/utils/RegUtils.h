
/********************************************************************
	created:	2016/03/14
	created:	14:3:2016   17:36
	filename: 	base\baselib\basehelper\utils\RegUtils.h
	file path:	base\baselib\basehelper\utils
	file base:	RegUtils
	file ext:	h
	author:		murisly
	
	purpose:	reg utils
*********************************************************************/

#pragma once


#include <windows.h>

namespace RegUtils
{
	//write reg value, not create key
	bool WriteRegString( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKey, LPCTSTR lpcsValue );
	bool WriteRegString( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsSubPath, LPCTSTR lpcsKey, LPCTSTR lpcsValue );
	bool WriteRegMultiSZ( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKey, LPCTSTR lpcsValue);
	bool WriteRegDWORD( HKEY hKey, LPCTSTR lpcsPath,LPCTSTR lpcsSubPath, LPCTSTR lpcsKey, DWORD dwValue );

	// read string to system registry
	bool ReadRegString( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKey, LPTSTR lpcsValue, DWORD cbSize = MAX_PATH );
	bool ReadRegDWORD( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKey, DWORD &dwValue );

	// check if the key is already exist
	bool IsKeyExist( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsName );

	bool DeleteRegValue(HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsName);
	bool DeleteKey( HKEY hKey, LPCTSTR lpcsPath );

	// create a path if it is not exist
	bool CreateKey( HKEY hKey, LPCTSTR lpcsPath,LPCTSTR lpcsSubPath );

	// write string to registry, if the key path is not exist, will create one
	bool WriteString( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKey, LPCTSTR lpcsValue );
	bool WriteDWORD( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKey, DWORD dwValue );
	bool WriteString( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsSubPath, LPCTSTR lpcsKey, LPCTSTR lpcsValue );
	bool WriteDWORD( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsSubPath, LPCTSTR lpcsKey, DWORD dwValue );
}
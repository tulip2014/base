

#include "RegUtils.h"
#include <string>
#include <Strsafe.h>
#include <Shlwapi.h>


namespace RegUtils{

	bool WriteRegString( HKEY h_Key, LPCTSTR lpcsPath, LPCTSTR lpcsKey, LPCTSTR lpcsValue )
	{
		bool bRet = false;
		HKEY hKey = NULL;

		if (ERROR_SUCCESS == ::RegOpenKeyEx(h_Key, lpcsPath, 0, KEY_SET_VALUE, &hKey))
		{
			std::wstring tValue = lpcsValue;
			if (ERROR_SUCCESS == ::RegSetValueEx(hKey, lpcsKey, 0, REG_SZ, (CONST BYTE*)tValue.c_str(), tValue.length() * sizeof( _TCHAR ) ))
			{
				bRet = true;
			}
			::RegCloseKey(hKey);
		}

		return bRet;
	}

	bool WriteRegString( HKEY h_Key, LPCTSTR lpcsPath, LPCTSTR lpcsSubPath, LPCTSTR lpcsKey, LPCTSTR lpcsValue )
	{
		bool bRet = false;
		HKEY hKey = NULL;
		HKEY hTempKey = NULL;

		if (ERROR_SUCCESS == ::RegOpenKeyEx(h_Key, lpcsPath, 0, KEY_SET_VALUE, &hKey))
		{
			if (ERROR_SUCCESS == ::RegCreateKey(hKey, lpcsSubPath, &hTempKey))
			{
				std::wstring tValue = lpcsValue;
				if (ERROR_SUCCESS == ::RegSetValueEx(hTempKey, lpcsKey, 0, REG_SZ, (CONST BYTE*)tValue.c_str(), tValue.length() * sizeof( _TCHAR ) ))
				{
					bRet = true;
				}
				::RegCloseKey(hTempKey);
			}
			::RegCloseKey(hKey);
		}
		return bRet;
	}

	bool WriteRegMultiSZ( HKEY h_Key, LPCTSTR lpcsPath, LPCTSTR lpcsKey, LPCTSTR lpcsValue)
	{
		bool bRet = false;
		HKEY hKey = NULL;

		if (ERROR_SUCCESS == ::RegOpenKeyEx(h_Key, lpcsPath, 0, KEY_SET_VALUE, &hKey))
		{
			std::wstring tValue = lpcsValue;
			if (ERROR_SUCCESS == ::RegSetValueEx(hKey, lpcsKey, 0, REG_MULTI_SZ, (CONST BYTE*)tValue.c_str(), tValue.length() * sizeof( _TCHAR ) ))
			{
				bRet = true;
			}
			::RegCloseKey(hKey);
		}

		return bRet;
	}


	bool ReadRegString( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKey, LPTSTR lpcsValue, DWORD cbSize )
	{
		bool bRet = false;
		HKEY hSubKey = NULL;
		if (ERROR_SUCCESS == ::RegOpenKeyEx( hKey, lpcsPath,0,KEY_READ,&hSubKey ) )
		{
			DWORD dwSzType = REG_SZ;
			if (ERROR_SUCCESS ==::RegQueryValueEx(hSubKey,lpcsKey,0,&dwSzType,(LPBYTE)lpcsValue, &cbSize))
			{
				bRet = true;
			}
			::RegCloseKey(hSubKey);
		}
		::RegCloseKey(hKey);
		return bRet;
	}

	bool ReadRegDWORD( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKey, DWORD &dwValue )
	{
		bool bRet = false;
		HKEY hSubKey = NULL;
		if (ERROR_SUCCESS == ::RegOpenKeyEx( hKey, lpcsPath, 0, KEY_READ, &hSubKey ) )
		{
			DWORD dwSzType = REG_DWORD;
			DWORD dwSize = sizeof (DWORD);
			if (ERROR_SUCCESS == ::RegQueryValueEx(hSubKey, lpcsKey, 0, &dwSzType, (LPBYTE)&dwValue, &dwSize))
			{
				bRet = true;
			}
			::RegCloseKey(hSubKey);
		}
		::RegCloseKey(hKey);
		return bRet;
	}

	//***************************************************************************************************
	// check if the key is already exist
	bool IsKeyExist( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKeyName )
	{
		HKEY hSubKey = NULL;
		_TCHAR tcsKey[MAX_PATH];
		if ( lpcsKeyName != NULL ) {
			StringCbPrintf( tcsKey, MAX_PATH* sizeof(_TCHAR), _T("%s\\%s"), lpcsPath, lpcsKeyName );
		}
		else	{
			StringCbPrintf( tcsKey, MAX_PATH* sizeof(_TCHAR), _T("%s"), lpcsPath );
		}

		if ( ERROR_SUCCESS == ::RegOpenKeyEx( hKey, tcsKey, 0, KEY_READ, &hSubKey)
			&& hSubKey != NULL )
		{
			::RegCloseKey( hSubKey );
			return true;
		}
		return false;
	}

	bool DeleteRegValue( HKEY h_Key, LPCTSTR lpcsPath, LPCTSTR lpcsName )
	{
		bool bRet = false;
		HKEY hKey = NULL;
		if (ERROR_SUCCESS == ::RegOpenKeyEx(h_Key, lpcsPath, 0, KEY_SET_VALUE, &hKey))
		{
			if (ERROR_SUCCESS == ::RegDeleteValue(hKey, lpcsName))
			{
				bRet = true;
			}
			::RegCloseKey(hKey);
		}

		return bRet;
	}

	//***************************************************************************************************
	// Deletes a subkey and all its descendants. This function removes the key and all the key's values from the registry.
	bool DeleteKey( HKEY hKey, LPCTSTR lpcsPath )
	{
		if ( IsKeyExist( hKey, lpcsPath, NULL ) )
		{
			// Deletes a subkey and all its descendants. SHDeleteKey removes the key and all the key's values from the registry.
			if ( ERROR_SUCCESS != ::SHDeleteKey( hKey, lpcsPath ) )
			{
				DWORD dwLastError = GetLastError();
				return false;
			}
		}
		return true;
	}

	bool CreateKey( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsSubPath )
	{
		if ( lpcsPath == NULL || _tcslen( lpcsPath ) < 1 )	{
			_ASSERT( 0 );
			return false;
		}

		_TCHAR tcsKey[MAX_PATH];
		if ( lpcsSubPath != NULL ) {
			StringCbPrintf( tcsKey, MAX_PATH* sizeof(_TCHAR), _T("%s\\%s"), lpcsPath, lpcsSubPath );
		}
		else	{
			StringCbPrintf( tcsKey, MAX_PATH* sizeof(_TCHAR), _T("%s"), lpcsPath );
		}

		HKEY hSubKey = NULL;
		// Creates the specified registry key. If the key already exists, the function opens it. Note that key names are not case sensitive.
		// An application cannot create a key that is a direct child of HKEY_USERS or HKEY_LOCAL_MACHINE
		// The RegCreateKeyEx function creates all missing keys in the specified path. An application can take advantage of this behavior to create several keys at once
		// If your service or application impersonates different users, do not use this function with HKEY_CURRENT_USER. Instead, call the RegOpenCurrentUser function.
		LSTATUS lsError = ::RegCreateKeyEx( hKey, lpcsPath, 0, NULL, 0, KEY_WRITE, NULL, &hSubKey, NULL );
		if ( hSubKey == NULL )	{
			_ASSERT( 0 );
			return false;
		}
		::RegCloseKey( hSubKey );
		return true;
	}

	//***************************************************************************************************
	// write string to registry, if the key path is not exist, will create one
	bool WriteString( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsSubPath, LPCTSTR lpcsKey, LPCTSTR lpcsValue )
	{
		if ( lpcsPath == NULL || _tcslen( lpcsPath ) < 1 )	{
			_ASSERT( 0 );
			return false;
		}

		if ( lpcsSubPath != NULL )	{
			_TCHAR tcsPath[MAX_PATH];
			StringCbPrintf( tcsPath, MAX_PATH* sizeof(_TCHAR), _T("%s\\%s"), lpcsPath, lpcsSubPath );
			return WriteString( hKey, tcsPath, lpcsKey, lpcsValue );
		}
		else {
			return WriteString( hKey, lpcsPath, lpcsKey, lpcsValue );
		}
	}

	//***************************************************************************************************
	// write string to registry, if the key path is not exist, will create one
	bool WriteDWORD( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsSubPath, LPCTSTR lpcsKey, DWORD dwValue )
	{
		if ( lpcsPath == NULL || _tcslen( lpcsPath ) < 1 )	{
			_ASSERT( 0 );
			return false;
		}

		if ( lpcsSubPath != NULL )	{
			_TCHAR tcsPath[MAX_PATH];
			StringCbPrintf( tcsPath, MAX_PATH* sizeof(_TCHAR), _T("%s\\%s"), lpcsPath, lpcsSubPath );
			return WriteDWORD( hKey, tcsPath, lpcsKey, dwValue );
		}
		else {
			return WriteDWORD( hKey, lpcsPath, lpcsKey, dwValue );
		}
	}


	//***************************************************************************************************
	// write string to registry, if the key path is not exist, will create one
	bool WriteString( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKey, LPCTSTR lpcsValue )
	{
		HKEY hSubKey = NULL;
		// Creates the specified registry key. If the key already exists, the function opens it. Note that key names are not case sensitive.
		// An application cannot create a key that is a direct child of HKEY_USERS or HKEY_LOCAL_MACHINE
		// The RegCreateKeyEx function creates all missing keys in the specified path. An application can take advantage of this behavior to create several keys at once
		// If your service or application impersonates different users, do not use this function with HKEY_CURRENT_USER. Instead, call the RegOpenCurrentUser function.
		LSTATUS lsError = ::RegCreateKeyEx( hKey, lpcsPath, 0, NULL, 0, KEY_WRITE, NULL, &hSubKey, NULL );
		if ( hSubKey == NULL )	{
			_ASSERT( 0 );
			return false;
		}

		// update key-value
		lsError = ::RegSetValueEx( hSubKey, lpcsKey, 0, REG_SZ, (LPBYTE)lpcsValue, _tcslen( lpcsValue ) * sizeof(_TCHAR ) );
		::RegCloseKey( hSubKey );
		return ( lsError == ERROR_SUCCESS ) ? true : false;
	}

	//***************************************************************************************************
	// write string to registry, if the key path is not exist, will create one
	bool WriteDWORD( HKEY hKey, LPCTSTR lpcsPath, LPCTSTR lpcsKey, DWORD dwValue )
	{
		HKEY hSubKey = NULL;
		LSTATUS lsError = ::RegCreateKeyEx( hKey, lpcsPath, 0, NULL, 0, KEY_WRITE, NULL, &hSubKey, NULL );
		if ( hSubKey == NULL )	{
			_ASSERT( 0 );
			return false;
		}

		// update key-value
		lsError = ::RegSetValueEx( hSubKey, lpcsKey, 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD) );
		::RegCloseKey( hSubKey );
		return ( lsError == ERROR_SUCCESS ) ? true : false;
	}
}
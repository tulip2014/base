#include "stdafx.h"
#include "MyPipe.h"
#include <time.h>
#include "CommonFunc.h"

int CMyPipe::StaticSendBufferLength = 0;
int CMyPipe::StaticReceiveBufferLength = 0;
HANDLE CMyPipe::StaticExitEvent = NULL;
int CMyPipe::StaticFlag = 0;

CMyPipe::CMyPipe()
{
	dwArg0 = 0;
	dwArg1 = -1;
	dwArg2 = 3000;
	dwArg3 = 3000;
	dwArg4 = 3000;
	memset(dwArg20, 0, 0x64);
	dwArg30 = 0;
	dwArg31 = 0;
	memset(dwArg128, 0, 0x14);
	hdArg37 = 0;
	lpc_section38 = 0;
}

CMyPipe::~CMyPipe()
{
	close();
}

int CMyPipe::ClearCriticalSection()
{
	LPCRITICAL_SECTION* v3;
	if ( lpc_section38 )
	{
		DeleteCriticalSection(*lpc_section38);
		v3 = (LPCRITICAL_SECTION*)lpc_section38;
		delete(v3);
		lpc_section38 = 0;
	}
	return 1;
}

int CMyPipe::ClearLocaleValue()
{
	ClearOverlapped();
	if ( hdArg37 )
	{
		CloseHandle(hdArg37);
		hdArg37 = 0;
	}
	ClearCriticalSection();
	return 1;
}

int CMyPipe::ClearOverlapped()
{
	if ( overlap.hEvent )
		CloseHandle(overlap.hEvent);
	memset(dwArg128, 0, 0x14u);
	return 1;
}

int ClearPipe()
{
	return CMyPipe::clear();
}

int CMyPipe::CreateCriticalSection()
{
	int result;
	LPCRITICAL_SECTION* v3 = NULL;
	v3 = new LPCRITICAL_SECTION;

	lpc_section38 = v3;
	if ( lpc_section38 )
	{
		InitializeCriticalSection(*lpc_section38);
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}

HANDLE CMyPipe::CreateNewPipe(LPCWSTR lpName, int a3)
{
	HANDLE result;
	HANDLE hObject;
	struct _SECURITY_ATTRIBUTES SecurityAttributes;
	CMyPipe *v7;

	v7 = this;
	SecurityAttributes.nLength = 0;
	SecurityAttributes.lpSecurityDescriptor = 0;
	SecurityAttributes.bInheritHandle = 0;
	if ( CreateSecurityAttributes(&SecurityAttributes, 2u) )
	{
		hObject = CreateNamedPipeW(
			lpName,
			0x40000003u,
			6u,
			0xFFu,
			CMyPipe::StaticSendBufferLength,
			CMyPipe::StaticReceiveBufferLength,
			0xBB8u,
			&SecurityAttributes);
		if ( a3 && hObject != (HANDLE)-1 && GetLastError() == 183 )
		{
			CloseHandle(hObject);
			hObject = (HANDLE)-1;
		}
		FreeSecurityAttributes(&SecurityAttributes);
		result = hObject;
	}
	else
	{
		result = 0;
	}
	return result;
}

int CMyPipe::CreateOverlapped()
{
	int result;

	if ( hdArg36 )
	{
		result = 1;
	}
	else
	{
		memset(dwArg128, 0, 0x14u);
		hdArg36 = CreateEventW(0, 0, 0, 0);  //36是event
		result = ( hdArg36 != 0 );
	}
	return result;
}

void CMyPipe::GetRandomName(wchar_t *Dst, int SizeInWords)
{
	unsigned __int32 v3;
	v3 = GetTickCount();
	_stprintf_s(Dst, SizeInWords, L"\\\\.\\pipe\\%08X", v3);
}

int CMyPipe::InitialLocaleValue()
{
	int result; // eax@4
	HANDLE v2; // [sp+Ch] [bp-D0h]@1

	if ( CreateOverlapped()
		&& (v2 = CreateEventW(0, 0, 0, 0), (hdArg37 = v2) != 0)
		&& CreateCriticalSection() )
	{
		result = 1;
	}
	else
	{
		ClearLocaleValue();
		result = 0;
	}
	return result;
}

int InitialPipe(unsigned __int32 a1, unsigned __int32 a2)
{
	return CMyPipe::initial(a1, a2);
}

int CMyPipe::ResetOverlapped()
{
	HANDLE v3 = hdArg36;
	memset((char *)this + 128, 0, 0x14u);
	hdArg36 = v3;
	return 1;
}



// 等待客户端连接，然后创建新的管道
int CMyPipe::accept(CMyPipe *hFile)
{
	int result; // eax@2
	DWORD v3;
	DWORD NumberOfBytesTransferred; // [sp+E0h] [bp-44h]@20
	HANDLE Handles; // [sp+ECh] [bp-38h]@11
	HANDLE v7; // [sp+F0h] [bp-34h]@11
	HANDLE v8; // [sp+F4h] [bp-30h]@11
	DWORD v9; // [sp+100h] [bp-24h]@8
	int v10; // [sp+10Ch] [bp-18h]@7
	CMyPipe *v11; // [sp+118h] [bp-Ch]@1


	v11 = this;
	if ( CMyPipe::StaticFlag == 2 )
	{
		if ( hFile != INVALID_HANDLE_VALUE && overlap.hEvent && dwArg30 & 1 )
		{
			v10 = 0;
			dwArg30 = dwArg30 & 0xFFFFFFFD | 2;
			if ( !ConnectNamedPipe(dwArg31, &overlap ))
			{
				v9 = GetLastError();
				if ( v9 == 535 )
				{
					v10 = 0;
				}
				else if ( v9 == 997 )
				{
					Handles = CMyPipe::StaticExitEvent;
					v7 = overlap.hEvent;
					v8 = overlap.hEvent;
					v3 = WaitForMultipleObjects(3u, &Handles, 0, dwArg1);
					if ( v3 > 258 )
					{
						if ( v3 == -1 )
							v10 = -536863738;
					}
					else if ( v3 == 258 )
					{
						CancelIo(*((HANDLE *)v11 + 31));
						v10 = -536863732;
					}
					else if ( v3 <= 1 )
					{
						v10 = -536863733;
					}
					else if ( v3 == 2 )
					{
						NumberOfBytesTransferred = 0;
						if ( GetOverlappedResult((HANDLE)hFile, (LPOVERLAPPED)((char *)v11 + 128), &NumberOfBytesTransferred, 0) )
							v10 = 0;
						else
							v10 = -536863733;
					}
				}
				else
				{
					v10 = -536863738;
				}
			}

			*((DWORD *)v11 + 30) &= 0xFFFFFFFD;
			if ( v10 )
			{
				result = v10;
			}
			else
			{
				hFile->dwArg31 = CreateNewPipe((LPCWSTR)v11 + 10, 0);
				if ( hFile->dwArg31 != INVALID_HANDLE_VALUE && hFile->InitialLocaleValue() )
				{
					v4 = *((DWORD *)hFile + 31);
					*((DWORD *)hFile + 31) = *((DWORD *)v11 + 31);
					*((DWORD *)v11 + 31) = v4;
					*((DWORD *)hFile + 30) = *((DWORD *)hFile + 30) & 0xFFFFFFFE | 1;
					*((DWORD *)hFile + 30) = *((DWORD *)hFile + 30) & 0xFFFFFFFB | 4;
					*((DWORD *)v11 + 30) = *((DWORD *)v11 + 30) & 0xFFFFFFFE | 1;
					*((DWORD *)v11 + 30) &= 0xFFFFFFFD;
					result = 0;
				}
				else
				{
					DisconnectNamedPipe(*((HANDLE *)v11 + 31));
					result = -536863739;
				}
			}
		}
		else
		{
			result = -536863735;
		}
	}
	else
	{
		result = -536863735;
	}
	return result;
}

int CMyPipe::clear()
{
	if ( CMyPipe::StaticExitEvent )
	{
		SetEvent(CMyPipe::StaticExitEvent);
		CloseHandle(CMyPipe::StaticExitEvent);
		CMyPipe::StaticExitEvent = 0;
	}
	return 0;
}

int CMyPipe::close()
{
	if ( hdArg37 )
		SetEvent(hdArg37);
	if ( dwArg31 != (HANDLE)-1 )
	{
		CloseHandle(dwArg31);
		dwArg31 = 0;
	}
	ClearLocaleValue();
	return 0;
}

int CMyPipe::connect(const wchar_t *Str)
{
	int result; // eax@2
	DWORD Mode; // [sp+D0h] [bp-38h]@14
	time_t v5; // [sp+DCh] [bp-2Ch]@8
	struct _SECURITY_ATTRIBUTES SecurityAttributes; // [sp+E8h] [bp-20h]@6
	CMyPipe *v7; // [sp+FCh] [bp-Ch]@1

	v7 = this;
	if ( CMyPipe::StaticFlag == 2 )
	{
		if ( Str && _tcslen(Str) )
		{
			*((DWORD *)v7 + 30) &= 0xFFFFFFFE;
			lstrcpynW((LPWSTR)v7 + 10, Str, 50);
			SecurityAttributes.nLength = 0;
			SecurityAttributes.lpSecurityDescriptor = 0;
			SecurityAttributes.bInheritHandle = 0;
			if ( CreateSecurityAttributes(&SecurityAttributes, 2u) )
			{
				v5 = time(0);
				while ( 1 )
				{
					dwArg31 = CreateFileW(Str, 0xC0000000, 0, &SecurityAttributes, 3u, 0x40000000u, 0);
					if ( *((DWORD *)v7 + 31) != -1 || time(0) - v5 > *((DWORD *)v7 + 2) / 1000 )
						break;
					WaitNamedPipeW(Str, *((DWORD *)v7 + 2));
					Sleep(0x64u);
				}
				FreeSecurityAttributes(&SecurityAttributes);
				if ( *((DWORD *)v7 + 31) == -1 )
				{
					result = -536863738;
				}
				else
				{
					Mode = 2;
					if ( SetNamedPipeHandleState(*((HANDLE *)v7 + 31), &Mode, 0, 0) )
					{
						if ( InitialLocaleValue() )
						{
							*((DWORD *)v7 + 30) = *((DWORD *)v7 + 30) & 0xFFFFFFFB | 4;
							result = 0;
						}
						else
						{
							DisconnectNamedPipe(*((HANDLE *)v7 + 31));
							CloseHandle(*((HANDLE *)v7 + 31));
							*((DWORD *)v7 + 31) = -1;
							result = -536863738;
						}
					}
					else
					{
						CloseHandle(*((HANDLE *)v7 + 31));
						result = -536863738;
					}
				}
			}
			else
			{
				result = -536863739;
			}
		}
		else
		{
			result = -536863742;
		}
	}
	else
	{
		result = -536863735;
	}
	return result;
}

int StopPipe()
{
	return CMyPipe::stop();
}

int CMyPipe::initial(unsigned __int32 a1, unsigned __int32 a2)
{
	int result;

	if ( a1 )
		CMyPipe::StaticSendBufferLength = a1;
	if ( a2 )
		CMyPipe::StaticReceiveBufferLength = a2;
	CMyPipe::StaticExitEvent = CreateEventW(0, 1, 0, 0);
	if ( CMyPipe::StaticExitEvent )
	{
		CMyPipe::StaticFlag = 2;
		result = 0;
	}
	else
	{
		result = 0xE0001C10;
	}
	return result;
}

//创建管道
int CMyPipe::listen(wchar_t *Str, int a3)
{
	int result; // eax@2
	char v4; // [sp+Ch] [bp-D8h]@1
	int i; // [sp+D0h] [bp-14h]@8
	CMyPipe *v6; // [sp+DCh] [bp-8h]@1

	memset(&v4, 0xCCu, 0xD8u);
	v6 = this;
	if ( CMyPipe::StaticFlag != 2 )
		return -536863735;
	if ( !Str )
		return -536863742;
	if ( _tcslen(Str) )
	{
		dwArg31 = CreateNewPipe(Str, 1);
	}
	else
	{
		if ( a3 < 20 )
			return -536863742;
		for ( i = 0; ; ++i )
		{
			if ( i < 10 )
			{
				GetRandomName(Str, a3);
				dwArg31 = CreateNewPipe(Str, 1);
				if ( *((DWORD *)v6 + 31) == -1 )
					continue;
			}
			break;
		}
	}

	if ( dwArg31 == (HANDLE)-1 )
	{
		result = -536863739;
	}
	else if ( InitialLocaleValue() )
	{
		lstrcpynW((LPWSTR)v6 + 10, Str, 50);
		*((DWORD *)v6 + 30) = *((DWORD *)v6 + 30) & 0xFFFFFFFE | 1;
		result = 0;
	}
	else
	{
		CloseHandle(*((HANDLE *)v6 + 31));
		*((DWORD *)v6 + 31) = -1;
		result = -536863728;
	}
	return result;
}


//从管道读取字节信息
int CMyPipe::receive(void *lpBuffer, int *a3, int a4)
{
	int result; // eax@4
	DWORD v5; // [sp+Ch] [bp-110h]@1
	DWORD dwMilliseconds; // [sp+10h] [bp-10Ch]@12
	HANDLE Handles; // [sp+D8h] [bp-44h]@11
	int v8; // [sp+DCh] [bp-40h]@11
	int v9; // [sp+E0h] [bp-3Ch]@11
	DWORD v10; // [sp+ECh] [bp-30h]@10
	DWORD NumberOfBytesRead; // [sp+F8h] [bp-24h]@9
	int v12; // [sp+104h] [bp-18h]@9
	CMyPipe *v13; // [sp+110h] [bp-Ch]@1
	unsigned int v14; // [sp+118h] [bp-4h]@1
	int savedregs; // [sp+11Ch] [bp+0h]@1


	v13 = this;
	if ( lpBuffer && a3 && *a3 )
	{
		if ( *((DWORD *)v13 + 31) != -1 && *((DWORD *)v13 + 36) && *((DWORD *)v13 + 30) & 4 )
		{
			v12 = 0;
			NumberOfBytesRead = 0;
			if ( !ReadFile(*((HANDLE *)v13 + 31), lpBuffer, *a3, &NumberOfBytesRead, (LPOVERLAPPED)((char *)v13 + 128)) )
			{
				v10 = GetLastError();
				if ( v10 == 997 )
				{
					Handles = CMyPipe::StaticExitEvent;
					v8 = *((DWORD *)v13 + 37);
					v9 = *((DWORD *)v13 + 36);
					if ( a4 )
						dwMilliseconds = a4;
					else
						dwMilliseconds = *((DWORD *)v13 + 4);
					v5 = WaitForMultipleObjects(3u, &Handles, 0, dwMilliseconds);
					if ( v5 > 0x102 )
					{
						if ( v5 == -1 )
							v12 = -536863736;
					}
					else if ( v5 == 258 )
					{
						CancelIo(*((HANDLE *)v13 + 31));
						v12 = -536863730;
					}
					else if ( v5 <= 1 )
					{
						v12 = -536863733;
					}
					else if ( v5 == 2 )
					{
						if ( GetOverlappedResult(*((HANDLE *)v13 + 31), (LPOVERLAPPED)((char *)v13 + 128), &NumberOfBytesRead, 0) )
							v12 = 0;
						else
							v12 = -536863733;
					}
				}
				else if ( v10 != 109 && v10 != 232 && v10 != 233 )
				{
					v12 = -536863736;
				}
				else
				{
					v12 = -536863733;
				}
			}
			if ( v12 )
				*a3 = 0;
			else
				*a3 = NumberOfBytesRead;
			ResetOverlapped();
			result = v12;
		}
		else
		{
			result = -536863735;
		}
	}
	else
	{
		result = -536863742;
	}
	return result;
}

int CMyPipe::send(LPCVOID lpBuffer, int nNumberOfBytesToWrite)
{
	int result; // eax@3
	DWORD v4; // [sp+Ch] [bp-10Ch]@1
	HANDLE Handles; // [sp+D4h] [bp-44h]@10
	int v6; // [sp+D8h] [bp-40h]@10
	int v7; // [sp+DCh] [bp-3Ch]@10
	DWORD v8; // [sp+E8h] [bp-30h]@9
	DWORD NumberOfBytesWritten; // [sp+F4h] [bp-24h]@8
	int v10; // [sp+100h] [bp-18h]@8
	CMyPipe *v11; // [sp+10Ch] [bp-Ch]@1
	unsigned int v12; // [sp+114h] [bp-4h]@1
	int savedregs; // [sp+118h] [bp+0h]@1

	v11 = this;
	if ( lpBuffer && nNumberOfBytesToWrite )
	{
		if ( *((DWORD *)v11 + 31) != -1 && *((DWORD *)v11 + 36) && *((DWORD *)v11 + 30) & 4 )
		{
			v10 = 0;
			NumberOfBytesWritten = 0;
			if ( !WriteFile(
				*((HANDLE *)v11 + 31),
				lpBuffer,
				nNumberOfBytesToWrite,
				&NumberOfBytesWritten,
				(LPOVERLAPPED)((char *)v11 + 128)) )
			{
				v8 = GetLastError();
				if ( v8 == 997 )
				{
					Handles = CMyPipe::StaticExitEvent;
					v6 = *((DWORD *)v11 + 37);
					v7 = *((DWORD *)v11 + 36);
					v4 = WaitForMultipleObjects(3u, &Handles, 0, *((DWORD *)v11 + 3));
					if ( v4 > 0x102 )
					{
						if ( v4 == -1 )
							v10 = -536863737;
					}
					else if ( v4 == 258 )
					{
						CancelIo(*((HANDLE *)v11 + 31));
						v10 = -536863731;
					}
					else if ( v4 <= 1 )
					{
						v10 = -536863733;
					}
					else if ( v4 == 2 )
					{
						if ( GetOverlappedResult(*((HANDLE *)v11 + 31), (LPOVERLAPPED)((char *)v11 + 128), &NumberOfBytesWritten, 0) )
							v10 = 0;
						else
							v10 = -536863733;
					}
				}
				else if ( v8 != 109 && v8 != 232 && v8 != 233 )
				{
					v10 = -536863737;
				}
				else
				{
					v10 = -536863733;
				}
			}
			ResetOverlapped();
			result = v10;
		}
		else
		{
			result = -536863735;
		}
	}
	else
	{
		result = -536863742;
	}
	return result;
}

int CMyPipe::set(int a2, int a3, int a4, int a5, int a6)
{
	CMyPipe *v7; // [sp+D0h] [bp-8h]@1

	v7 = this;
	*(DWORD *)this = a2;
	*((DWORD *)v7 + 1) = a3;
	*((DWORD *)v7 + 2) = a4;
	*((DWORD *)v7 + 3) = a5;
	*((DWORD *)v7 + 4) = a6;
	return 0;
}

int CMyPipe::stop()
{
	if ( CMyPipe::StaticExitEvent )
		SetEvent(CMyPipe::StaticExitEvent);
	CMyPipe::StaticFlag = 1;
	return 0;
}

BOOL CMyPipe::IsConnected()
{
	return (dwArg30 & 4) != 0;
}
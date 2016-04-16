
#include "stdafx.h"
#include "InterFunc.h"
#include "CommonFunc.h"

int PCL_1001(unsigned __int32 a1, unsigned __int32 *a2)
{
	return 0;
	//return Temp_CalcSelfClientID(a1, a2);
}

int PCL_1002(unsigned __int32 a1, void *Dst)
{
	int result; // eax@2
	size_t v3; // eax@6
	char v4; // [sp+Ch] [bp-2E8h]@1
	DWORD pcbBuffer; // [sp+D0h] [bp-224h]@5
	WCHAR Buffer; // [sp+DCh] [bp-218h]@5
	char v7; // [sp+DEh] [bp-216h]@5
	int v8; // [sp+2E8h] [bp-Ch]@3
	unsigned int v9; // [sp+2F0h] [bp-4h]@1
	int savedregs; // [sp+2F4h] [bp+0h]@1

	memset(&v4, 0xCCu, 0x2E8u);
	v9 = (unsigned int)&savedregs ^ __security_cookie;
	if ( Dst )
	{
		memset(Dst, 0, 0x21Cu);
		v8 = Temp_CalcSelfClientID(a1, (unsigned __int32 *)Dst + 1);
		if ( v8 )
		{
			result = v8;
		}
		else
		{
			*(_DWORD *)Dst = 0;
			*((_DWORD *)Dst + 3) = GetCurrentProcessId();
			ProcessIdToSessionId(*((_DWORD *)Dst + 3), (DWORD *)Dst + 2);
			GetModuleFileNameW(0, (LPWSTR)Dst + 10, 0x104u);
			Buffer = 0;
			memset_0(&v7, 0, 0x200u);
			pcbBuffer = 257;
			if ( GetUserNameW(&Buffer, &pcbBuffer) <= 0 )
			{
				*((_DWORD *)Dst + 4) = -1;
			}
			else
			{
				__wcsupr_s(&Buffer, 0x101u);
				v3 = _wcslen(&Buffer);
				*((_DWORD *)Dst + 4) = CalcCRC32(2 * v3, &Buffer);
			}
			result = 0;
		}
	}
	else
	{
		result = -536863742;
	}
	return result;
}


int PCL_1004(unsigned __int32 a1, unsigned __int32 a2, unsigned __int32 a3, unsigned __int32 a4, unsigned __int32 a5, struct tagPCL_BUFFER *a6, struct tagPCL_BUFFER *a7)
{
	return Temp_SendCommand(a1, a2, a3, a4, a5, a6, a7);
}
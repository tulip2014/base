#include "stdafx.h"
#include "PipeCommuLib.h"
#include "CommonFunc.h"
#include "PipeComm.h"

int PCL_0001(PPCL_MODULEINFO Src)
{
	int result; // eax@3
	char v2; // [sp+Ch] [bp-10Ch]@1
	PPCL_MODULEINFO v3; // [sp+10h] [bp-108h]@7
	unsigned __int32 v4; // [sp+DCh] [bp-3Ch]@26
	unsigned __int32 v5; // [sp+E8h] [bp-30h]@26
	struct CMyPipe *v6; // [sp+F4h] [bp-24h]@26
	int v7; // [sp+100h] [bp-18h]@21
	PPCL_MODULEINFO Dst; // [sp+10Ch] [bp-Ch]@7
	unsigned int v9; // [sp+114h] [bp-4h]@1
	int savedregs; // [sp+118h] [bp+0h]@1

	if ( Src && Src->m_dwSize <= 0x30u )
	{
		if ( Src->m_FuncAddr[0] && Src->m_FuncAddr[1] )
		{
			v3 = new PCL_MODULEINFO();
			Dst = v3;
			if ( v3 )
			{
				memset(Dst, 0, 0x30u);
				memmove(Dst, Src, Src->m_dwSize);
				if ( !Dst->m_dwConnectTimeout )
					Dst->m_dwConnectTimeout = 20000;
				if ( !Dst->m_dwSendTimeout )
					Dst->m_dwSendTimeout = 20000;
				if ( !Dst->m_dwRecvTimeout )
					Dst->m_dwRecvTimeout = 20000;
				if ( !Dst->m_FuncAddr[2] )
					Dst->m_FuncAddr[2] = &Temp_TreatQueryUserInfo;
				if ( !Dst->m_FuncAddr[3] )
					Dst->m_FuncAddr[3] = &Temp_TreatSendCommand;
				if ( !Dst->m_FuncAddr[4] )
					Dst->m_FuncAddr[4] = &Temp_TreatOpReturn;
				*((_DWORD *)&g_globalData + 49) = Dst;
				v7 = InitialGlobalData();
				if ( v7 )
				{
					Temp_Close();
					result = v7;
				}
				else
				{
					v7 = CreatePipeServerThread();
					if ( v7 )
					{
						Temp_Close();
						result = v7;
					}
					else if ( (*((_DWORD *)&g_globalData + 8) & 0xFFFF) != 1
						&& (v6 = 0, v5 = 0, v4 = 1, (v7 = ClientIDToPipe(&v4, &v6, &v5)) != 0) )
					{
						Temp_Close();
						result = v7;
					}
					else
					{
						result = 0;
					}
				}
			}
			else
			{
				result = -536863743;
			}
		}
		else
		{
			result = -536863742;
		}
	}
	else
	{
		result = -536863742;
	}
	return result;
}

int PCL_0002()
{
	return Temp_Close();
}

void* PCL_0003(unsigned int a1)
{
	return Temp_malloc(a1);
}

void* PCL_0004(unsigned int a1, unsigned int a2)
{
	return Temp_calloc(a1, a2);
}

void PCL_0005(void *a1)
{
	Temp_free(a1);
}

void* PCL_0006(void *a1, unsigned int a2)
{
	return Temp_realloc(a1, a2);
}
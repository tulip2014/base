#include "PipeComm.h"
#include "PipeCommuLib.h"
#include "CommonFunc.h"
#include "Packet.h"
#include "MyPipe.h"

int CalcClientID( PCLIENTINFO a1)
{
	a1->dwFlag = a1->dwFlag & 0xFFFF | (CRC16((unsigned __int8 *)a1, 0xCu) << 16) & 0xFFFF0000;
	return 1;
}

int SendPacketData(unsigned __int32 a1, unsigned __int32 a2, unsigned __int32 a3, CPacket *a4, CPacket *a5, unsigned __int32 a6)
{
	int result; // eax@6
	int v8; // [sp+D0h] [bp-30h]@13
	int v9; // [sp+DCh] [bp-24h]@7
	unsigned __int32 v10; // [sp+E8h] [bp-18h]@7
	DWORD v11; // [sp+F4h] [bp-Ch]@7
	unsigned int v12; // [sp+FCh] [bp-4h]@1
	int savedregs; // [sp+100h] [bp+0h]@1

	if ( (*((DWORD *)&g_globalData + 8) & 0xFFFF) != 1
		|| (a1 & 0xFFFF0000) == -65536
		|| (unsigned __int16)a1 == 0xFFFF
		|| a1 & 0xFFFF0000
		|| (unsigned __int16)a1 == 2 )
	{
		v11 = GetTickCount();
		v10 = GetCycleCount();
		v9 = a4->TreatSendPacketData(a1, a2, a3, a6);
		if ( !v9 )
		{
			if ( (*((DWORD *)&g_globalData + 8) & 0xFFFF) == 1 )
			{
				v8 = 0;
				if ( (a1 & 0xFFFF0000) != -65536 && (unsigned __int16)a1 != 0xFFFF )
					v9 = SendPacketData(0, a1, a4, a5);
				else
					v9 = SendBroadcastPacket(a1, a4, a5, *((_DWORD *)&g_globalData + 8), &v8);
			}
			else
			{
				if ( (a1 & 0xFFFF0000) == -65536 || (unsigned __int16)a1 == 0xFFFF )
					a1 = 1;
				v9 = SendPacketData(0, a1, a4, a5);
			}
		}
		result = v9;
	}
	else
	{
		result = -536863742;
	}
	return result;
}


int SendPacketData(CMyPipe *a1, unsigned __int32 a2, CPacket *a3, CPacket *a4)
{
	int v5; // [sp+Ch] [bp-110h]@1
	int v6; // [sp+D4h] [bp-48h]@1
	unsigned __int32 v7; // [sp+E0h] [bp-3Ch]@1
	unsigned __int32 v8; // [sp+ECh] [bp-30h]@1
	int v9; // [sp+F8h] [bp-24h]@1
	unsigned int v10; // [sp+100h] [bp-1Ch]@1

	v9 = 0;
	v8 = 0;
	v7 = a2;
	v5 = a1 != 0;
	v6 = a1 != 0;
	while ( v8 < 4 )
	{
		v7 = a2;
		if ( !a1 )
		{
			v9 = ClientIDToPipe(&v7, &a1, &v8);
			if ( v9 || !a1 )
				break;
		}
		v9 = TreatSendPacketData(a3, v7);
		if ( v9 )
			return v9;
		v9 = SendPacketData(a1, a3, a4);
		if ( v9 != -536863733 )
			break;
		if ( v6 )
			return v9;
		ms_exc.registration.TryLevel = 0;
		CloseClientPipe(v7);
		ms_exc.registration.TryLevel = -2;
		a1 = 0;
	}
	return v9;
}

int SendBroadcastPacket(unsigned __int32 a1, CPacket *a2, CPacket *a3, unsigned __int32 a4, int *a5)
{
	char v6; // [sp+Ch] [bp-1BCh]@1
	char v7; // [sp+10h] [bp-1B8h]@5
	unsigned int j; // [sp+DCh] [bp-ECh]@25
	unsigned int i; // [sp+E8h] [bp-E0h]@11
	int v10; // [sp+F4h] [bp-D4h]@11
	struct tagTHREADINFO *v11; // [sp+100h] [bp-C8h]@5
	unsigned int v12; // [sp+10Ch] [bp-BCh]@5
	unsigned __int32 v13; // [sp+118h] [bp-B0h]@5
	char v14; // [sp+11Ch] [bp-ACh]@5
	struct CMyPipe *v15; // [sp+170h] [bp-58h]@5
	char Dst; // [sp+174h] [bp-54h]@5
	unsigned int v17; // [sp+1C4h] [bp-4h]@1
	int savedregs; // [sp+1C8h] [bp+0h]@1

	memset(&v6, 0xCCu, 0x1BCu);
	v17 = (unsigned int)&savedregs ^ __security_cookie;
	if ( a1 == -1 || (a1 & 0xFFFF0000) == 0xFFFF && (unsigned __int16)a1 == 1 )
		*a5 = 1;
	v15 = 0;
	memset_0(&Dst, 0, 0x4Cu);
	v13 = 0;
	memset_0(&v14, 0, 0x4Cu);
	v12 = 0;
	CSafeCS::CSafeCS(&CriticalSection);
	CSafeCS::~CSafeCS((CSafeCS *)&v7);
	v11 = 0;
	if ( (unsigned __int16)a1 == 0xFFFF && !(a1 & 0xFFFF0000) )
	{
		v11 = FindThreadInfo(a4);
		if ( !v11 )
			return -536863742;
		if ( !*((_DWORD *)v11 + 1) )
			*a5 = 1;
	}
	v10 = byte_290;
	for ( i = 0; i < dword_294; ++i )
	{
		if ( (!a4 || *(_DWORD *)(v10 + (i << 7)) != a4)
			&& (*(_DWORD *)(v10 + (i << 7)) & 0xFFFF) != 1
			&& (a1 == -1
			|| (a1 & 0xFFFF0000) == -65536 && (unsigned __int16)a1 == (*(_DWORD *)(v10 + (i << 7)) & 0xFFFF)
			|| (unsigned __int16)a1 == 0xFFFF && v11 && *((_DWORD *)v11 + 1) == *(_DWORD *)(v10 + (i << 7) + 4)) )
		{
			*(&v15 + v12) = *(struct CMyPipe **)(v10 + (i << 7) + 24);
			*(&v13 + v12++) = *(_DWORD *)(v10 + (i << 7));
		}
	}
	for ( j = 0; j < v12; ++j )
		SendPacketData(*(&v15 + j), *(&v13 + j), a2, a3);
	return 0;
}

int TreatSendPacketData(CPacket *a1, unsigned __int32 a2)
{
	int result; // eax@2
	char v3; // [sp+Ch] [bp-CCh]@1
	unsigned __int8 *v4; // [sp+D0h] [bp-8h]@1

	memset(&v3, 0xCCu, 0xCCu);
	v4 = CPacket::GetData(a1);
	if ( a2 == *((_DWORD *)v4 + 7) )
	{
		result = 0;
	}
	else if ( (unsigned __int16)a2 != 1
		|| (*((_DWORD *)v4 + 7) & 0xFFFF0000) != -65536 && (*((_DWORD *)v4 + 7) & 0xFFFF) != 0xFFFF )
	{
		result = a1->TreatSendPacketData(a2);
	}
	else
	{
		result = 0;
	}
	return result;
}

int SendPacketData(struct CMyPipe *a1, struct CPacket *a2, struct CPacket *a3)
{
	struct _RTL_CRITICAL_SECTION *v3; // eax@1
	int result; // eax@4
	unsigned __int8 *v5; // esi@8
	char v6; // [sp+Ch] [bp-1130h]@1
	int v7; // [sp+10h] [bp-112Ch]@4
	int v8; // [sp+1Ch] [bp-1120h]@7
	int v9; // [sp+28h] [bp-1114h]@9
	int v10; // [sp+34h] [bp-1108h]@10
	char v11; // [sp+100h] [bp-103Ch]@5
	int v12; // [sp+1118h] [bp-24h]@1
	char v13; // [sp+1124h] [bp-18h]@1
	unsigned int v14; // [sp+112Ch] [bp-10h]@1
	int v15; // [sp+1138h] [bp-4h]@1
	int savedregs; // [sp+113Ch] [bp+0h]@1

	memset(&v6, 0xCCu, 0x1124u);
	v14 = (unsigned int)&savedregs ^ __security_cookie;
	CSafeCS::__autoclassinit((CSafeCS *)&v13, 1u);
	v3 = a1->GetCtriticalSection();
	CSafeCS::CSafeCS(v3);
	v15 = 0;
	v12 = SendPipePacket(a1, a2);
	if ( v12 )
	{
		v10 = v12;
		v15 = -1;
		CSafeCS::~CSafeCS((CSafeCS *)&v13);
		result = v10;
	}
	else if ( *((_DWORD *)CPacket::GetData(a2) + 10) & 1 || *((_DWORD *)CPacket::GetData(a2) + 10) & 2 )
	{
		v7 = v12;
		v15 = -1;
		CSafeCS::~CSafeCS((CSafeCS *)&v13);
		result = v7;
	}
	else
	{
		CPacket::__autoclassinit((CPacket *)&v11, 1u);
		CPacket::CPacket(&v11);
		LOBYTE(v15) = 1;
		do
		{
			v12 = ReceivePipePacket(a1, (struct CPacket *)&v11, 0);
			if ( v12 )
			{
				v8 = v12;
				LOBYTE(v15) = 0;
				CPacket::~CPacket((CPacket *)&v11);
				v15 = -1;
				CSafeCS::~CSafeCS((CSafeCS *)&v13);
				return v8;
			}
			v5 = CPacket::GetData(a2);
		}
		while ( *((_DWORD *)v5 + 1) != *((_DWORD *)CPacket::GetData((CPacket *)&v11) + 1) );
		v9 = CPacket::TreatSendPacketReturn((CPacket *)&v11, a3);
		LOBYTE(v15) = 0;
		CPacket::~CPacket((CPacket *)&v11);
		v15 = -1;
		CSafeCS::~CSafeCS((CSafeCS *)&v13);
		result = v9;
	}
	return result;
}

int SendPipePacket( CMyPipe *a1,  CPacket *a2)
{
	int v3; // [sp+Ch] [bp-100h]@1
	int v4; // [sp+D4h] [bp-38h]@1
	int v5; // [sp+E0h] [bp-2Ch]@1
	unsigned int v6; // [sp+ECh] [bp-20h]@1
	unsigned int v7; // [sp+F8h] [bp-14h]@1
	unsigned __int8 *v8; // [sp+104h] [bp-8h]@1

	memset(&v3, 0xCCu, 0x100u);
	v8 = CPacket::GetData(a2);
	v7 = *((_DWORD *)v8 + 2);
	v6 = 0;
	v5 = 0;
	v4 = 0;
	while ( v6 < v7 )
	{
		if ( v7 - v6 <= 0x1000 )
			v3 = v7 - v6;
		else
			v3 = 4096;
		v5 = v3;
		v4 = a1->send(&v8[v6], v3);
		if ( v4 )
			return v4;
		v6 += v5;
	}
	return 0;
};

DWORD CreatePipeServerThread()
{
	DWORD result; // eax@2
	DWORD v1; // [sp+Ch] [bp-FCh]@1
	DWORD v2; // [sp+D4h] [bp-34h]@5
	DWORD ExitCode; // [sp+E0h] [bp-28h]@5
	DWORD ThreadId; // [sp+ECh] [bp-1Ch]@3
	LPVOID lpParameter; // [sp+F8h] [bp-10h]@1
	HANDLE v6; // [sp+FCh] [bp-Ch]@1
	unsigned int v7; // [sp+104h] [bp-4h]@1
	int savedregs; // [sp+108h] [bp+0h]@1

	lpParameter = 0;
	v6 = 0;
	lpParameter = CreateEventW(0, 0, 0, 0);
	if ( !lpParameter )
		return -536863728;
	ThreadId = 0;
	v6 = CreateThread(0, 0, PipeServerThread, lpParameter, 0, &ThreadId);
	if ( !v6 )
	{
		CloseHandle(lpParameter);
		return -536863734;
	}
	*((_DWORD *)&g_globalData + 12) = v6;
	ExitCode = 0;
	v2 = WaitForMultipleObjects(2u, &lpParameter, 0, 0xFFFFFFFF);
	v1 = v2;
	if ( v2 <= 0x102 )
	{
		if ( v1 != 258 )
		{
			if ( v1 )
			{
				if ( v1 == 1 )
					GetExitCodeThread(v6, &ExitCode);
			}
			else
			{
				ExitCode = 0;
			}
			goto LABEL_14;
		}
		goto LABEL_11;
	}
	if ( v1 == -1 )
LABEL_11:
	ExitCode = -536863735;
LABEL_14:
	if ( lpParameter )
	{
		CloseHandle(lpParameter);
		lpParameter = 0;
	}
	if ( ExitCode )
	{
		*((_DWORD *)&g_globalData + 12) = 0;
		if ( v6 )
		{
			CloseHandle(v6);
			v6 = 0;
		}
		result = ExitCode;
	}
	else
	{
		result = 0;
	}
	return result;
}


DWORD WINAPI PipeServerThread(LPVOID lpThreadParameter)
{
	signed int result; // eax@5
	unsigned int v2; // [sp-4h] [bp-1F8h]@1
	int v3; // [sp+Ch] [bp-1E8h]@1
	int v4; // [sp+14h] [bp-1E0h]@4
	CMyPipe *v5; // [sp+20h] [bp-1D4h]@1
	CMyPipe *v6; // [sp+2Ch] [bp-1C8h]@9
	CMyPipe *v7; // [sp+38h] [bp-1BCh]@9
	CMyPipe *v8; // [sp+44h] [bp-1B0h]@18
	CMyPipe *v9; // [sp+50h] [bp-1A4h]@18
	int v10; // [sp+5Ch] [bp-198h]@28
	CMyPipe *v11; // [sp+68h] [bp-18Ch]@25
	struct CMyPipe *v12; // [sp+74h] [bp-180h]@33
	struct CMyPipe *v13; // [sp+80h] [bp-174h]@33
	void *v14; // [sp+8Ch] [bp-168h]@37
	struct CMyPipe *v15; // [sp+98h] [bp-15Ch]@38
	struct CMyPipe *v16; // [sp+A4h] [bp-150h]@38
	struct CMyPipe *v17; // [sp+B0h] [bp-144h]@43
	struct CMyPipe *v18; // [sp+BCh] [bp-138h]@43
	void *v19; // [sp+C8h] [bp-12Ch]@46
	HANDLE hThread; // [sp+194h] [bp-60h]@42
	void *Dst; // [sp+1A0h] [bp-54h]@37
	struct CMyPipe *v22; // [sp+1ACh] [bp-48h]@28
	DWORD ThreadId; // [sp+1B8h] [bp-3Ch]@24
	wchar_t *v24; // [sp+1C4h] [bp-30h]@14
	CMyPipe *v25; // [sp+1D0h] [bp-24h]@4
	int v26; // [sp+1DCh] [bp-18h]@1
	unsigned int v27; // [sp+1E4h] [bp-10h]@1
	int v28; // [sp+1F0h] [bp-4h]@1
	int savedregs; // [sp+1F4h] [bp+0h]@1

	memset(&v3, 0xCCu, 0x1DCu);
	v27 = (unsigned int)&savedregs ^ __security_cookie;
	v2 = (unsigned int)&savedregs ^ __security_cookie;
	v26 = 0;
	v5 = new CMyPipe();
	v28 = 0;
	if ( v5 )
	{
		CMyPipe::__autoclassinit(v5, 1u);
		v3 = CMyPipe(v5);
	}
	else
	{
		v3 = 0;
	}
	v4 = v3;
	v28 = -1;
	v25 = (CMyPipe *)v3;
	if ( v3 )
	{
		v26 = CMyPipe::set(
			v25,
			0,
			-1,
			*(_DWORD *)(*((_DWORD *)&g_globalData + 49) + 12),
			*(_DWORD *)(*((_DWORD *)&g_globalData + 49) + 16),
			*(_DWORD *)(*((_DWORD *)&g_globalData + 49) + 20));
		if ( v26 )
		{
			result = v26;
		}
		else
		{
			v26 = v25->listen((wchar_t *)&g_globalData + 30, 50);
			if ( v26 )
			{
				v7 = v25;
				v6 = v25;
				if ( v25 )
					v3 = CMyPipe::`scalar deleting destructor'(1);
				else
				v3 = 0;
				v25 = 0;
				result = v26;
			}
			else if ( (*((_DWORD *)&g_globalData + 8) & 0xFFFF) == 1
				&& ((v24 = wcsrchr((wchar_t *)&g_globalData + 30, 0x5Cu)) == 0 ? (v24 = (wchar_t *)((char *)&g_globalData
				+ 60)) : ++v24,
				(v26 = TrySetPipeName(v24)) != 0) )
			{
				v9 = v25;
				v8 = v25;
				if ( v25 )
					v3 = CMyPipe::`scalar deleting destructor'(1);
				else
				v3 = 0;
				v25 = 0;
				result = -536863725;
			}
			else
			{
				*((_DWORD *)&g_globalData + 13) = v25;
				if ( lpThreadParameter )
					SetEvent(lpThreadParameter);
				ThreadId = 0;
				while ( 1 )
				{
					v11 = (CMyPipe *)operator new(0x9Cu);
					v28 = 1;
					if ( v11 )
					{
						CMyPipe::__autoclassinit(v11, 1u);
						v3 = CMyPipe::CMyPipe(v11);
					}
					else
					{
						v3 = 0;
					}
					v10 = v3;
					v28 = -1;
					v22 = (struct CMyPipe *)v3;
					if ( !v3 )
						return -536863743;
					v26 = v22->set(
						0,
						-1,
						*(DWORD *)(*((DWORD *)&g_globalData + 49) + 12),
						*(DWORD *)(*((DWORD *)&g_globalData + 49) + 16),
						*(DWORD *)(*((DWORD *)&g_globalData + 49) + 20));
					if ( v26 )
						return v26;
					v26 = v25->accept(v22); //新创建的管道实例
					if ( v26 )
						break;
					v14 = operator new(0x80u);
					Dst = v14;
					if ( v14 )
					{
						memset(Dst, 0, 0x80u);
						*((DWORD *)Dst + 5) = v22;
						hThread = CreateThread(0, 0, PipeClientThread, Dst, 4u, &ThreadId);
						if ( hThread )
						{
							*((_DWORD *)Dst + 4) = hThread;
							ResumeThread(hThread);
						}
						else
						{
							v18 = v22;
							v17 = v22;
							if ( v22 )
								v3 = CMyPipe::`scalar deleting destructor'(1);
							else
							v3 = 0;
							v22 = 0;
							v19 = Dst;
							operator delete(Dst);
							if ( v19 )
							{
								Dst = (void *)33059;
								v3 = 33059;
							}
							else
							{
								v3 = 0;
							}
							Dst = 0;
						}
					}
					else
					{
						v16 = v22;
						v15 = v22;
						if ( v22 )
							v3 = CMyPipe::`scalar deleting destructor'(1);
						else
						v3 = 0;
						v22 = 0;
					}
				}
				v13 = v22;
				v12 = v22;
				if ( v22 )
					v3 = CMyPipe::`scalar deleting destructor'(1);
				else
				v3 = 0;
				v22 = 0;
				result = v26;
			}
		}
	}
	else
	{
		result = -536863743;
	}
	return result;
}

DWORD WINAPI PipeClientThread(LPVOID lpThreadParameter)
{
	signed int result; // eax@4
	int v2; // [sp+Ch] [bp-214Ch]@1
	void *v3; // [sp+14h] [bp-2144h]@6
	void *v4; // [sp+20h] [bp-2138h]@17
	int v5; // [sp+2Ch] [bp-212Ch]@20
	char v6; // [sp+F8h] [bp-2060h]@10
	char v7; // [sp+1110h] [bp-1048h]@10
	struct CMyPipe *v8; // [sp+2128h] [bp-30h]@10
	int v9; // [sp+2134h] [bp-24h]@5
	struct tagTHREADINFO *v10; // [sp+2140h] [bp-18h]@1
	unsigned int v11; // [sp+2148h] [bp-10h]@1
	int v12; // [sp+2154h] [bp-4h]@10
	int savedregs; // [sp+2158h] [bp+0h]@1

	v10 = (struct tagTHREADINFO *)lpThreadParameter;
	if ( lpThreadParameter && *((DWORD *)v10 + 5) && *((DWORD *)v10 + 4) )
	{
		v9 = WaitClientLogIn(v10);
		if ( v9 )
		{
			ClearThreadInfo(v10, 0, 0);
			v3 = (void *)v10;
			operator delete((void *)v10);
			if ( v3 )
			{
				v10 = (struct tagTHREADINFO *)33059;
				v2 = 33059;
			}
			else
			{
				v2 = 0;
			}
			result = v9;
		}
		else
		{
			v8 = (struct CMyPipe *)*((_DWORD *)v10 + 5);
			CPacket::__autoclassinit((CPacket *)&v7, 1u);
			CPacket::CPacket(&v7);
			v12 = 0;
			CPacket::__autoclassinit((CPacket *)&v6, 1u);
			CPacket::CPacket(&v6);
			LOBYTE(v12) = 1;
			do
			{
				v9 = ReceivePipePacket(v8, (struct CPacket *)&v7, -1);
				if ( !v9 )
				{
					v9 = CPacket::TreatReceivePacketData((CPacket *)&v7, (struct CPacket *)&v6);
					if ( !v9
						&& !(*((_DWORD *)CPacket::GetData((CPacket *)&v7) + 10) & 1)
						&& !(*((_DWORD *)CPacket::GetData((CPacket *)&v7) + 10) & 2) )
						SendPipePacket(v8, (struct CPacket *)&v6);
				}
			}
			while ( v9 != -536863733 );
			DelThreadInfo(v10);
			v4 = (void *)v10;
			operator delete((void *)v10);
			if ( v4 )
			{
				v10 = (struct tagTHREADINFO *)33059;
				v2 = 33059;
			}
			else
			{
				v2 = 0;
			}
			v10 = 0;
			v5 = 0;
			LOBYTE(v12) = 0;
			CPacket::~CPacket((CPacket *)&v6);
			v12 = -1;
			CPacket::~CPacket((CPacket *)&v7);
			result = v5;
		}
	}
	else
	{
		result = -536863742;
	}
	return result;
}


int WaitClientLogIn(struct tagTHREADINFO *a1)
{
	int result; // eax@2
	unsigned __int8 *v2; // eax@5
	char v3; // [sp+Ch] [bp-2148h]@1
	int v4; // [sp+10h] [bp-2144h]@2
	int v5; // [sp+1Ch] [bp-2138h]@4
	int v6; // [sp+28h] [bp-212Ch]@12
	int v7; // [sp+34h] [bp-2120h]@13
	char v8; // [sp+100h] [bp-2054h]@10
	unsigned __int8 *v9; // [sp+1118h] [bp-103Ch]@5
	int v10; // [sp+1124h] [bp-1030h]@1
	char v11; // [sp+1130h] [bp-1024h]@1
	unsigned int v12; // [sp+2144h] [bp-10h]@1
	int v13; // [sp+2150h] [bp-4h]@1
	int savedregs; // [sp+2154h] [bp+0h]@1

	CPacket::CPacket(&v11);
	v13 = 0;
	v10 = ReceivePipePacket(*((struct CMyPipe **)a1 + 5), (struct CPacket *)&v11, 0);
	if ( v10 )
	{
		v4 = v10;
		v13 = -1;
		CPacket::~CPacket((CPacket *)&v11);
		result = v4;
	}
	else
	{
		v10 = CPacket::CheckReceivePacketData((CPacket *)&v11);
		if ( v10 )
		{
			v5 = v10;
			v13 = -1;
			CPacket::~CPacket((CPacket *)&v11);
			result = v5;
		}
		else
		{
			v2 = CPacket::GetData((CPacket *)&v11);
			v9 = v2 + 48;
			v2 += 48;
			*(_DWORD *)a1 = *(_DWORD *)v2;
			*((_DWORD *)a1 + 1) = *((_DWORD *)v2 + 1);
			*((_DWORD *)a1 + 2) = *((_DWORD *)v2 + 2);
			*((_DWORD *)a1 + 3) = *((_DWORD *)v2 + 3);
			_memmove((char *)a1 + 28, v9 + 28, 0x64u);
			if ( *((_DWORD *)&g_globalData + 8) == *(_DWORD *)a1 )
				v10 = -536863741;
			if ( !v10 )
				v10 = AddThreadInfo(a1, 0xBFu);
			if ( v10 )
			{
				CPacket::__autoclassinit((CPacket *)&v8, 1u);
				CPacket::CPacket(&v8);
				LOBYTE(v13) = 1;
				if ( !CPacket::MakeReturnCodePacket((CPacket *)&v11, (struct CPacket *)&v8, v10) )
					SendPipePacket(*((struct CMyPipe **)a1 + 5), (struct CPacket *)&v8);
				v6 = v10;
				LOBYTE(v13) = 0;
				CPacket::~CPacket((CPacket *)&v8);
				v13 = -1;
				CPacket::~CPacket((CPacket *)&v11);
				result = v6;
			}
			else
			{
				v7 = SendLoginPacketReturn(
					*((struct CMyPipe **)a1 + 5),
					(struct CPacket *)&v11,
					(struct tagTHREADINFO *)((char *)&g_globalData + 32));
				v13 = -1;
				CPacket::~CPacket((CPacket *)&v11);
				result = v7;
			}
		}
	}
	return result;
}

int ReceivePipePacket(struct CMyPipe *a1, struct CPacket *a2, int a3)
{
	unsigned __int32 v4; // esi@8
	char v5; // [sp+Ch] [bp-10E4h]@1
	int v6; // [sp+D0h] [bp-1020h]@1
	unsigned __int32 v7; // [sp+DCh] [bp-1014h]@1
	char v8; // [sp+E8h] [bp-1008h]@2
	unsigned int v9; // [sp+10ECh] [bp-4h]@1
	int savedregs; // [sp+10F0h] [bp+0h]@1

	memset(&v5, 0xCCu, 0x10E4u);
	v9 = (unsigned int)&savedregs ^ __security_cookie;
	CPacket::SetAddPos(a2, 0, 0);
	v7 = 4096;
	v6 = 0;
	do
	{
		v7 = 4096;
		v6 = a1->receive(&v8, (int *)&v7, a3);
		if ( v6 )
			return v6;
		v6 = a2->AddData(&v8, v7);
		if ( v6 )
			return v6;
		v6 = a2->PreCheckPacket();
		if ( v6 )
			return v6;
		v4 = CPacket::GetLength(a2);
	}
	while ( v4 < *((DWORD *)CPacket::GetData(a2) + 2) );
	CPacket::SetGetPos(a2, 48, 0);
	return v6;
}

int ClientIDToPipe(unsigned __int32 *a1, struct CMyPipe **a2, unsigned __int32 *a3)
{
	char v4; // [sp+Ch] [bp-164h]@1
	size_t v5; // [sp+D0h] [bp-A0h]@25
	unsigned __int32 v6; // [sp+DCh] [bp-94h]@6
	char Dst; // [sp+E0h] [bp-90h]@6
	struct CMyPipe *v8; // [sp+F4h] [bp-7Ch]@14
	wchar_t Str[54]; // [sp+F8h] [bp-78h]@17
	int v10; // [sp+164h] [bp-Ch]@6
	unsigned int v11; // [sp+16Ch] [bp-4h]@1
	int savedregs; // [sp+170h] [bp+0h]@1

	memset(&v4, 0xCCu, 0x164u);
	v11 = (unsigned int)&savedregs ^ __security_cookie;
	if ( !a1 || *a1 == *(_DWORD *)(*((_DWORD *)&g_globalData + 49) + 8) || !a2 || !a3 )
		return -536863742;
	v10 = 0;
	v6 = 0;
	memset_0(&Dst, 0, 0x7Cu);
	v6 = *a1;
	while ( *a3 < 4 )
	{
		if ( *a3 >= 2 )
		{
			if ( (*((_DWORD *)&g_globalData + 8) & 0xFFFF) == 1 )
				return -536863724;
			if ( (*a1 & 0xFFFF) == 1 )
			{
				if ( *a3 >= 3 )
					return -536863724;
				_wcscpy_s(Str, 0x32u, L"\\\\.\\pipe\\");
				v5 = _wcslen(Str);
				v10 = TryGetPipeName(&Str[v5], 50 - v5);
				if ( v10 )
					return -536863724;
				*a3 = 3;
			}
			else
			{
				if ( *a3 >= 4 )
					return -536863724;
				v10 = QueryUserInfo(0, 1u, *a1, (struct tagTHREADINFO *)&v6);
				if ( v10 )
					return -536863724;
				if ( !_wcslen(Str) )
					return -536863740;
				*a3 = 4;
			}
LABEL_37:
			if ( *a3 == 2 || *a3 == 3 || *a3 == 4 )
			{
				if ( *((_DWORD *)&g_globalData + 1) != 2 )
					return -536863733;
				v10 = ConnectToPipeServer((struct tagTHREADINFO *)&v6);
				if ( !v10 )
				{
					v10 = GetThreadInfo((struct tagTHREADINFO *)&v6, 1u);
					if ( !v10 )
					{
						*a1 = v6;
						*a2 = v8;
						return v10;
					}
				}
			}
		}
		else
		{
			v10 = GetThreadInfo((struct tagTHREADINFO *)&v6, 1u);
			if ( v10 && (*((_DWORD *)&g_globalData + 8) & 0xFFFF) == 1 )
				return v10;
			if ( v10 )
			{
				*a3 = 2;
			}
			else
			{
				if ( !*a3 && v8 )
				{
					*a1 = v6;
					*a2 = v8;
					*a3 = 1;
					return v10;
				}
				if ( *a3 < 2 && _wcslen(Str) )
				{
					*a3 = 2;
					goto LABEL_37;
				}
				*a3 = 2;
			}
		}
	}
	return v10;
}

int ConnectToPipeServer(struct tagTHREADINFO *a1)
{
	int result; // eax@3
	int v2; // [sp+Ch] [bp-13Ch]@1
	int v3; // [sp+14h] [bp-134h]@9
	CMyPipe *v4; // [sp+20h] [bp-128h]@6
	int v5; // [sp+2Ch] [bp-11Ch]@12
	int v6; // [sp+38h] [bp-110h]@12
	int v7; // [sp+44h] [bp-104h]@17
	int v8; // [sp+50h] [bp-F8h]@17
	int v9; // [sp+5Ch] [bp-ECh]@24
	int v10; // [sp+68h] [bp-E0h]@24
	int v11; // [sp+134h] [bp-14h]@11
	int v12; // [sp+144h] [bp-4h]@6

	memset(&v2, 0xCCu, 0x130u);
	if ( a1 && _wcslen((const wchar_t *)a1 + 14) )
	{
		if ( CheckAndConnectClientInfo(*(_DWORD *)a1) )
		{
			v4 = (CMyPipe *)operator new(0x9Cu);
			v12 = 0;
			if ( v4 )
			{
				CMyPipe::__autoclassinit(v4, 1u);
				v2 = CMyPipe::CMyPipe(v4);
			}
			else
			{
				v2 = 0;
			}
			v3 = v2;
			v12 = -1;
			*((_DWORD *)a1 + 6) = v2;
			if ( *((_DWORD *)a1 + 6) )
			{
				v11 = CMyPipe::set(
					*((CMyPipe **)a1 + 6),
					0,
					-1,
					*(_DWORD *)(*((_DWORD *)&g_globalData + 49) + 12),
					*(_DWORD *)(*((_DWORD *)&g_globalData + 49) + 16),
					*(_DWORD *)(*((_DWORD *)&g_globalData + 49) + 20));
				if ( v11 )
				{
					v6 = *((_DWORD *)a1 + 6);
					v5 = v6;
					if ( v6 )
						v2 = CMyPipe::`scalar deleting destructor'(1);
					else
					v2 = 0;
					*((_DWORD *)a1 + 6) = 0;
					result = v11;
				}
				else
				{
					v11 = CMyPipe::connect(*((CMyPipe **)a1 + 6), (const wchar_t *)a1 + 14);
					if ( v11 )
					{
						v8 = *((_DWORD *)a1 + 6);
						v7 = v8;
						if ( v8 )
							v2 = CMyPipe::`scalar deleting destructor'(1);
						else
						v2 = 0;
						*((_DWORD *)a1 + 6) = 0;
						result = v11;
					}
					else
					{
						v11 = SendLoginPacket(*((struct CMyPipe **)a1 + 6), *(_DWORD *)a1, a1);
						if ( v11 )
						{
							if ( CMyPipe::IsConnected(*((CMyPipe **)a1 + 6)) )
								SendLogOutPacket(*((struct CMyPipe **)a1 + 6), *(_DWORD *)a1);
							v10 = *((_DWORD *)a1 + 6);
							v9 = v10;
							if ( v10 )
								v2 = CMyPipe::`scalar deleting destructor'(1);
							else
							v2 = 0;
							*((_DWORD *)a1 + 6) = 0;
							result = v11;
						}
						else
						{
							v11 = AddThreadInfo(a1, 0xCFu);
							DelConnectClientInfo(*(_DWORD *)a1);
							result = v11;
						}
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
			result = 0;
		}
	}
	else
	{
		result = -536863742;
	}
	return result;
}
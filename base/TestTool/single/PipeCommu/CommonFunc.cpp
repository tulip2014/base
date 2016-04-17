
#include "stdafx.h"
#include "CommonFunc.h"
#include "PipeCommuLib.h"
#include "InterFunc.h"
#include "MyPipe.h"
#include "PipeComm.h"

int Temp_TreatQueryUserInfo(unsigned __int32 a1, unsigned __int32 a2, unsigned __int32 a3, PPCL_CLIENTINFO a4)
{
	int result;
	if ( a4 )
		result = PCL_1002(a3, a4);
	else
		result = -536863742;
	return result;
}

int Temp_Close()
{
	ClearGlobalData();
	return 0;
}

int Temp_CalcSelfClientID(unsigned __int32 a1, unsigned __int32 *a2)
{
	int result; // eax@2
	char v3; // [sp+Ch] [bp-DCh]@1
	unsigned __int32 v4; // [sp+D0h] [bp-18h]@3
	DWORD pSessionId; // [sp+D4h] [bp-14h]@3
	DWORD dwProcessId; // [sp+D8h] [bp-10h]@3
	int v7; // [sp+DCh] [bp-Ch]@3
	unsigned int v8; // [sp+E4h] [bp-4h]@1
	int savedregs; // [sp+E8h] [bp+0h]@1

	if ( a2 )
	{
		pSessionId = 0;
		dwProcessId = 0;
		v7 = 0;
		v4 = a1;
		dwProcessId = GetCurrentProcessId();
		ProcessIdToSessionId(dwProcessId, &pSessionId);
		v7 = 0;
		CalcClientID((PCLIENTINFO)&v4);
		*a2 = v4;
		result = 0;
	}
	else
	{
		result = -536863742;
	}
	return result;
}

int Temp_TreatSendCommand()
{
	return 0;
}

int Temp_TreatOpReturn()
{
	return 0;
}

int TryCloseAllThreadInfo1()
{
	char v1; // [sp+Ch] [bp-FCh]@1
	char v2; // [sp+10h] [bp-F8h]@3
	unsigned int j; // [sp+DCh] [bp-2Ch]@3
	int v4; // [sp+E8h] [bp-20h]@3
	int v5; // [sp+F4h] [bp-14h]@3
	unsigned int i; // [sp+100h] [bp-8h]@1

	memset(&v1, 0xCCu, 0xFCu);
	for ( i = 0; ; ++i )
	{
		if ( i < 3 )
		{
			Sleep(0xC8u);
			CSafeCS::CSafeCS(&CriticalSection);
			CSafeCS::~CSafeCS((CSafeCS *)&v2);
			v5 = 0;
			v4 = dword_290;
			for ( j = 0; j < dword_294; ++j )
			{
				if ( *(_DWORD *)(v4 + (j << 7) + 20) )
					++v5;
			}
			if ( v5 )
				continue;
		}
		break;
	}
	return 1;
}

int TryCloseAllThreadInfo1Ex()
{
	return TryCloseAllThreadInfo1();
}

int ClearThreadInfo(struct tagTHREADINFO *a1, int a2, int a3)
{
	int result; // eax@2
	int v4; // [sp+Ch] [bp-F4h]@1
	int v5; // [sp+14h] [bp-ECh]@11
	int v6; // [sp+20h] [bp-E0h]@11
	int v7; // [sp+2Ch] [bp-D4h]@19
	int v8; // [sp+38h] [bp-C8h]@19

	memset(&v4, 0xCCu, 0xF4u);
	if ( a1 )
	{
		if ( *((_DWORD *)a1 + 4) )
		{
			if ( a2 )
			{
				TerminateThread(*((HANDLE *)a1 + 4), 0);
				WaitForSingleObject(*((HANDLE *)a1 + 4), 0x1F4u);
			}
			CloseHandle(*((HANDLE *)a1 + 4));
			*((_DWORD *)a1 + 4) = 0;
		}
		if ( *((_DWORD *)a1 + 5) )
		{
			if ( a3 && CMyPipe::IsConnected(*((CMyPipe **)a1 + 5)) )
				SendLogOutPacket(*((struct CMyPipe **)a1 + 5), *(_DWORD *)a1);
			v6 = *((_DWORD *)a1 + 5);
			v5 = v6;
			if ( v6 )
				v4 = CMyPipe::`scalar deleting destructor'(1);
			else
			v4 = 0;
			*((_DWORD *)a1 + 5) = 0;
		}
		if ( *((_DWORD *)a1 + 6) )
		{
			if ( a3 && CMyPipe::IsConnected(*((CMyPipe **)a1 + 6)) )
				SendLogOutPacket(*((struct CMyPipe **)a1 + 6), *(_DWORD *)a1);
			v8 = *((_DWORD *)a1 + 6);
			v7 = v8;
			if ( v8 )
				v4 = CMyPipe::`scalar deleting destructor'(1);
			else
			v4 = 0;
			*((_DWORD *)a1 + 6) = 0;
		}
		memset((char *)a1 + 28, 0, 0x64u);
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}

int TryCloseAllThreadInfo2()
{
	char v1; // [sp+Ch] [bp-FCh]@1
	unsigned int i; // [sp+E8h] [bp-20h]@1

	ClearThreadInfo((struct tagTHREADINFO *)&dword_210, 1, 1);
	for ( i = 0; i < dword_294; ++i )
		ClearThreadInfo((struct tagTHREADINFO *)((i << 7) + byte_290), 1, 1);
	return 1;
}

int CloseAllThreadInfo()
{
	char v2; // [sp+10h] [bp-C8h]@1

	StopPipe();
	TryCloseAllThreadInfo1Ex();
	return TryCloseAllThreadInfo2();
}

int ClearGlobalData()
{
	int result; // eax@2
	char v1; // [sp+Ch] [bp-CCh]@1
	void *v2; // [sp+10h] [bp-C8h]@4

	memset(&v1, 0xCCu, 0xCCu);
	if ( g_globalData )
	{
		dword_1F4 = 1;
		CloseAllThreadInfo();
		DeleteCriticalSection(&CriticalSection);
		ClearPointer((void **)&dword_290);
		if ( dword_2B4 )
		{
			v2 = (void *)dword_2B4;
			operator delete((void *)dword_2B4);
			dword_2B4 = 0;
		}
		memset(&g_globalData, 0, 0xC8u);
		result = 0;
	}
	else
	{
		result = 0;
	}
	return result;
}

int InitialGlobalData()
{
	int result; // eax@2
	int v2; // [sp+D0h] [bp-8h]@3

	ClearGlobalData();
	InitialThreadInfo((struct tagTHREADINFO *)&dword_210);
	*(_DWORD *)(dword_2B4 + 8) = dword_210;
	if ( AllocaBuf((struct tagSD_PARAM *)&dword_290, 0x80u, 2u) )
	{
		v2 = dword_290;
		*dword_290 = 1;
		*(_DWORD *)(v2 + 128) = 2;
		dword_294 = 2;
		memcpy(&unk_2A8, &m_InnerFuncAddress, 0xCu);
		InitializeCriticalSection(&CriticalSection);
		g_globalData = 808726855;
		dword_1F4 = 2;
		result = 0;
	}
	else
	{
		result = -536863743;
	}
	return result;
}

char crctable[] =
{
	0, 580h, 0F80h, 0A00h, 1B80h, 1E00h, 1400h, 1180h, 3380h
	3600h, 3C00h, 3980h, 2800h, 2D80h, 2780h, 2200h, 6380h
	6600h, 6C00h, 6980h, 7800h, 7D80h, 7780h, 7200h, 5000h
	5580h, 5F80h, 5A00h, 4B80h, 4E00h, 4400h, 4180h, 0C380h
	0C600h, 0CC00h, 0C980h, 0D800h, 0DD80h, 0D780h, 0D200h
	0F000h, 0F580h, 0FF80h, 0FA00h, 0EB80h, 0EE00h, 0E400h
	0E180h, 0A000h, 0A580h, 0AF80h, 0AA00h, 0BB80h, 0BE00h
	0B400h, 0B180h, 9380h, 9600h, 9C00h, 9980h, 8800h, 8D80h
	8780h, 8200h, 8381h, 8601h, 8C01h, 8981h, 9801h, 9D81h
	9781h, 9201h, 0B001h, 0B581h, 0BF81h, 0BA01h, 0AB81h
	0AE01h, 0A401h, 0A181h, 0E001h, 0E581h, 0EF81h, 0EA01h
	0FB81h, 0FE01h, 0F401h, 0F181h, 0D381h, 0D601h, 0DC01h
	0D981h, 0C801h, 0CD81h, 0C781h, 0C201h, 4001h, 4581h
	4F81h, 4A01h, 5B81h, 5E01h, 5401h, 5181h, 7381h, 7601h
	7C01h, 7981h, 6801h, 6D81h, 6781h, 6201h, 2381h, 2601h
	2C01h, 2981h, 3801h, 3D81h, 3781h, 3201h, 1001h, 1581h
	1F81h, 1A01h, 0B81h, 0E01h, 401h, 181h, 383h, 603h
	0C03h, 983h, 1803h, 1D83h, 1783h, 1203h, 3003h, 3583h
	3F83h, 3A03h, 2B83h, 2E03h, 2403h, 2183h, 6003h, 6583h
	6F83h, 6A03h, 7B83h, 7E03h, 7403h, 7183h, 5383h, 5603h
	5C03h, 5983h, 4803h, 4D83h, 4783h, 4203h, 0C003h, 0C583h
	0CF83h, 0CA03h, 0DB83h, 0DE03h, 0D403h, 0D183h, 0F383h
	0F603h, 0FC03h, 0F983h, 0E803h, 0ED83h, 0E783h, 0E203h
	0A383h, 0A603h, 0AC03h, 0A983h, 0B803h, 0BD83h, 0B783h
	0B203h, 9003h, 9583h, 9F83h, 9A03h, 8B83h, 8E03h, 8403h
	8183h, 8002h, 8582h, 8F82h, 8A02h, 9B82h, 9E02h, 9402h
	9182h, 0B382h, 0B602h, 0BC02h, 0B982h, 0A802h, 0AD82h
	0A782h, 0A202h, 0E382h, 0E602h, 0EC02h, 0E982h, 0F802h
	0FD82h, 0F782h, 0F202h, 0D002h, 0D582h, 0DF82h, 0DA02h
	0CB82h, 0CE02h, 0C402h, 0C182h, 4382h, 4602h, 4C02h
	4982h, 5802h, 5D82h, 5782h, 5202h, 7002h, 7582h, 7F82h
	7A02h, 6B82h, 6E02h, 6402h, 6182h, 2002h, 2582h, 2F82h
	2A02h, 3B82h, 3E02h, 3402h, 3182h, 1382h, 1602h, 1C02h
	1982h, 802h, 0D82h, 782h, 202h
};

int CRC16(unsigned __int8 *a1, unsigned __int32 a2)
{
	int result; // eax@3
	unsigned __int32 i; // [sp+D0h] [bp-2Ch]@4
	unsigned __int8 v5; // [sp+DFh] [bp-1Dh]@4
	unsigned __int8 v6; // [sp+EBh] [bp-11h]@4
	unsigned __int16 v7; // [sp+F4h] [bp-8h]@6

	if ( a1 && a2 )
	{
		v6 = 0;
		v5 = 0;
		for ( i = 0; i < a2; ++i )
		{
			v7 = a1[i];
			v7 ^= v6;
			v7 *= 2;
			v6 = crctable[v7] ^ v5;
			//? v5 = byte_2C9[v7];
		}
		result = v5 | (v6 << 8);
	}
	else
	{
		result = 0;
	}
	return result;
}

void* Temp_malloc(size_t Size)
{
	return malloc(Size);
}

void* Temp_calloc(size_t NumOfElements, size_t SizeOfElements)
{
	return calloc(NumOfElements, SizeOfElements);
}

void Temp_free(void *Memory)
{
	free(Memory);
}

void* Temp_realloc(void *Memory, size_t NewSize)
{
	return realloc(Memory, NewSize);
}

//接受客户端来的数据，然后发送出去
int Temp_SendCommand(unsigned __int32 a1, char a2, unsigned __int32 a3, char a4, char a5, struct tagPCL_BUFFER *a6, struct tagPCL_BUFFER *a7)
{
	char v8; // [sp+Ch] [bp-2148h]@1
	int v9; // [sp+10h] [bp-2144h]@6
	int v10; // [sp+1Ch] [bp-2138h]@10
	int v11; // [sp+28h] [bp-212Ch]@12
	int v12; // [sp+F4h] [bp-2060h]@4
	char v13; // [sp+100h] [bp-2054h]@4
	unsigned __int32 v14; // [sp+1118h] [bp-103Ch]@4
	int v15; // [sp+1124h] [bp-1030h]@3
	char v16; // [sp+1130h] [bp-1024h]@1
	unsigned int v17; // [sp+2144h] [bp-10h]@1
	int v18; // [sp+2150h] [bp-4h]@1
	int savedregs; // [sp+2154h] [bp+0h]@1

	memset(&v8, 0xCCu, 0x213Cu);
	v17 = (unsigned int)&savedregs ^ __security_cookie;
	CPacket::__autoclassinit((CPacket *)&v16, 1u);
	CPacket::CPacket(&v16);
	v18 = 0;
	CPacket::AddData<unsigned long>(&a2);
	CPacket::AddData<unsigned long>(&a3);
	CPacket::AddData<unsigned long>(&a4);
	CPacket::AddData<unsigned long>(&a5);
	if ( a6 )
	{
		CPacket::AddData<unsigned long>((void *)a6);
		CPacket::AddBuf((CPacket *)&v16, *((const void **)a6 + 1), *(_DWORD *)a6);
	}
	else
	{
		v15 = -1;
		CPacket::AddData<unsigned long>(&v15);
	}
	v14 = a3 & 1;
	CPacket::__autoclassinit((CPacket *)&v13, 1u);
	CPacket::CPacket(&v13);
	LOBYTE(v18) = 1;
	v12 = SendPacketData(a1, 3u, v14, (struct CPacket *)&v16, (struct CPacket *)&v13, 0);
	if ( v12 || !a7 )
	{
		v9 = v12;
		LOBYTE(v18) = 0;
		CPacket::~CPacket((CPacket *)&v13);
		v18 = -1;
		CPacket::~CPacket((CPacket *)&v16);
		return v9;
	}
	CPacket::GetData<unsigned long>((void *)a7);
	if ( *(_DWORD *)a7 )
	{
		*((_DWORD *)a7 + 1) = Temp_malloc(*(_DWORD *)a7);
		if ( !*((_DWORD *)a7 + 1) )
		{
			v10 = -536863743;
			LOBYTE(v18) = 0;
			CPacket::~CPacket((CPacket *)&v13);
			v18 = -1;
			CPacket::~CPacket((CPacket *)&v16);
			return v10;
		}
		CPacket::GetBuf((CPacket *)&v13, *((void **)a7 + 1), *(_DWORD *)a7);
	}
	else
	{
		*((_DWORD *)a7 + 1) = 0;
	}
	v11 = v12;
	LOBYTE(v18) = 0;
	CPacket::~CPacket((CPacket *)&v13);
	v18 = -1;
	CPacket::~CPacket((CPacket *)&v16);
	return v11;
}

unsigned __int64 GetCycleCount()
{
	return __rdtsc();
}

int CreateSecurityAttributes(_SECURITY_ATTRIBUTES *a1, unsigned __int32 a2)
{
	int v3; // [sp+Ch] [bp-D0h]@1
	PSECURITY_DESCRIPTOR pSecurityDescriptor; // [sp+D4h] [bp-8h]@3

	memset(&v3, 0xCCu, 0xD0u);
	if ( !a1 )
		return 0;
	a1->lpSecurityDescriptor = 0;
	pSecurityDescriptor = 0;
	if ( !(a2 & 2) )
	{
		a1->lpSecurityDescriptor = 0;
		goto LABEL_9;
	}
	pSecurityDescriptor = LocalAlloc(0x40u, 0x14u);
	if ( pSecurityDescriptor
		&& InitializeSecurityDescriptor(pSecurityDescriptor, 1u)
		&& SetSecurityDescriptorDacl(pSecurityDescriptor, 1, 0, 0) )
	{
		a1->lpSecurityDescriptor = pSecurityDescriptor;
LABEL_9:
		v3 = (a2 & 1) != 0;
		a1->bInheritHandle = v3;
		a1->nLength = 12;
		return 1;
	}
	return 0;
}

int FreeSecurityAttributes(struct _SECURITY_ATTRIBUTES *a1)
{
	char v2; // [sp+Ch] [bp-C0h]@1

	memset(&v2, 0xCCu, 0xC0u);
	if ( a1 && a1->lpSecurityDescriptor )
	{
		LocalFree(a1->lpSecurityDescriptor);
		a1->lpSecurityDescriptor = 0;
	}
	return 1;
}

int DelThreadInfo(struct tagTHREADINFO *a1)
{
	return TryDelThreadInfo(a1);
}

int TryDelThreadInfo(struct tagTHREADINFO *a1)
{
	char v2; // [sp+Ch] [bp-108h]@1
	int v3; // [sp+E8h] [bp-2Ch]@2
	void *Dst; // [sp+F4h] [bp-20h]@1

	Dst = (void *)FindThreadInfo(*(DWORD *)a1);
	if ( Dst )
	{
		ClearThreadInfo((struct tagTHREADINFO *)Dst, 0, 0);
		v3 = *(DWORD *)Dst & 0xFFFF;
		if ( v3 != 1 && v3 != 2 )
			memmove(Dst, (char *)Dst + 128, (dword_294-- << 7) + byte_290 - (_DWORD)((char *)Dst + 128));
	}
	return 0;
}

int FindThreadInfo(unsigned __int32 a1)
{
	int result; // eax@5
	int v2; // [sp+Ch] [bp-DCh]@1
	unsigned int i; // [sp+D4h] [bp-14h]@4
	int v4; // [sp+E0h] [bp-8h]@1

	memset(&v2, 0xCCu, 0xDCu);
	v4 = (unsigned __int16)a1;
	v2 = (unsigned __int16)a1;
	if ( (WORD)a1 )
	{
		if ( v2 == 1 )
		{
			result = byte_290;
		}
		else if ( v2 == 2 )
		{
			result = byte_290 + 128;
		}
		else
		{
			for ( i = 2; i < dword_294; ++i )
			{
				if ( *(_DWORD *)(byte_290 + (i << 7)) == a1
					|| !(a1 & 0xFFFF0000) && (*(_DWORD *)(byte_290 + (i << 7)) & 0xFFFF) == (unsigned __int16)a1 )
					return (i << 7) + byte_290;
			}
			result = 0;
		}
	}
	else
	{
		result = 0;
	}
	return result;
}

int SendLoginPacket(struct CMyPipe *a1, unsigned __int32 a2, struct tagTHREADINFO *a3)
{
	int result; // eax@2
	unsigned __int8 *v4; // eax@8
	char v5; // [sp+Ch] [bp-2148h]@1
	int v6; // [sp+10h] [bp-2144h]@2
	int v7; // [sp+1Ch] [bp-2138h]@4
	int v8; // [sp+28h] [bp-212Ch]@8
	int v9; // [sp+F4h] [bp-2060h]@6
	unsigned __int8 *v10; // [sp+100h] [bp-2054h]@5
	int v11; // [sp+10Ch] [bp-2048h]@1
	char v12; // [sp+118h] [bp-203Ch]@1
	char v13; // [sp+1130h] [bp-1024h]@1
	unsigned int v14; // [sp+2144h] [bp-10h]@1
	int v15; // [sp+2150h] [bp-4h]@1
	int savedregs; // [sp+2154h] [bp+0h]@1

	memset(&v5, 0xCCu, 0x213Cu);
	v14 = (unsigned int)&savedregs ^ __security_cookie;
	CPacket::__autoclassinit((CPacket *)&v13, 1u);
	CPacket::CPacket(&v13);
	v15 = 0;
	CPacket::AddData<tagTHREADINFO>(&dword_210);
	CPacket::__autoclassinit((CPacket *)&v12, 1u);
	CPacket::CPacket(&v12);
	LOBYTE(v15) = 1;
	v11 = SendPacketData(a1, a2, 0x80000000, 0, (struct CPacket *)&v13, (struct CPacket *)&v12, 0);
	if ( v11 )
	{
		v6 = v11;
		LOBYTE(v15) = 0;
		CPacket::~CPacket((CPacket *)&v12);
		v15 = -1;
		CPacket::~CPacket((CPacket *)&v13);
		result = v6;
	}
	else if ( CPacket::GetLength((CPacket *)&v12) == 176 )
	{
		CPacket::GetData<tagTHREADINFO *>(&v10);
		if ( v10 )
		{
			CPacket::GetData<unsigned long>(&v9);
			if ( v9 )
			{
				v10 = CPacket::GetCurrentPos((CPacket *)&v12);
				CPacket::SetGetPos((CPacket *)&v12, v9, 2u);
			}
		}
		v4 = v10;
		*(_DWORD *)a3 = *(_DWORD *)v10;
		*((_DWORD *)a3 + 1) = *((_DWORD *)v4 + 1);
		*((_DWORD *)a3 + 2) = *((_DWORD *)v4 + 2);
		*((_DWORD *)a3 + 3) = *((_DWORD *)v4 + 3);
		_memmove((char *)a3 + 28, v10 + 28, 0x64u);
		v8 = 0;
		LOBYTE(v15) = 0;
		CPacket::~CPacket((CPacket *)&v12);
		v15 = -1;
		CPacket::~CPacket((CPacket *)&v13);
		result = v8;
	}
	else
	{
		v7 = -536863740;
		LOBYTE(v15) = 0;
		CPacket::~CPacket((CPacket *)&v12);
		v15 = -1;
		CPacket::~CPacket((CPacket *)&v13);
		result = v7;
	}
	return result;
}
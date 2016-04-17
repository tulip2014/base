#include "stdafx.h"
#include "Packet.h"

CPacket::CPacket()
{
	dwArg0 = 48;
	dwArg1 = 4096;
	dwArg2 = 48;
	dwArg3 = 0;
}

CSafeCS::CSafeCS(LPCRITICAL_SECTION pcs)
{
	m_pcs = pcs;
	EnterCriticalSection(pcs);
}

CPacket::~CPacket()
{
	Empty();
}


int CPacket::AddBuf(const void *Src, unsigned __int32 a3)
{
	char v4; // [sp+Ch] [bp-D8h]@1
	int v5; // [sp+D0h] [bp-14h]@1
	CPacket *v6; // [sp+DCh] [bp-8h]@1

	memset(&v4, 0xCCu, 0xD8u);
	v6 = this;
	v5 = AddData<void const *>(&Src);
	if ( v5 )
		return v5;
	if ( Src )
	{
		v5 = CPacket::AddData<unsigned long>(v6, &a3);
		if ( v5 )
			return v5;
		if ( a3 )
			v5 = CPacket::AddData(v6, Src, a3);
	}
	return v5;
}

int CPacket::AddData(const void *Src, unsigned __int32 Size)
{
	int result; // eax@2
	unsigned __int8 *v4; // eax@3
	char v5; // [sp+Ch] [bp-CCh]@1
	CPacket *v6; // [sp+D0h] [bp-8h]@1

	memset(&v5, 0xCCu, 0xCCu);
	v6 = this;
	if ( CPacket::AllocBuf(this, Size) )
	{
		v4 = CPacket::GetBuf(v6);
		memmove(&v4[*(_DWORD *)v6], Src, Size);
		*(_DWORD *)v6 += Size;
		result = 0;
	}
	else
	{
		result = -536863743;
	}
	return result;
}

int CPacket::AllocBuf(unsigned __int32 a2)
{
	int result; // eax@3
	size_t v3; // ST08_4@9
	unsigned __int8 *v4; // eax@9
	char v5; // [sp+Ch] [bp-108h]@1
	void *v6; // [sp+10h] [bp-104h]@6
	void *v7; // [sp+1Ch] [bp-F8h]@11
	void *Dst; // [sp+E8h] [bp-2Ch]@6
	unsigned int v9; // [sp+F4h] [bp-20h]@4
	unsigned __int32 v10; // [sp+100h] [bp-14h]@1
	CPacket *v11; // [sp+10Ch] [bp-8h]@1

	memset(&v5, 0xCCu, 0x108u);
	v11 = this;
	v10 = (*(_DWORD *)this + a2 + 15) & 0xFFFFFFF0;
	if ( v10 > 0x1000 && v10 > *((_DWORD *)v11 + 1) )
	{
		v9 = 2 * *((_DWORD *)v11 + 1);
		if ( v9 < v10 )
			v9 = v10;
		v6 = operator new(v9);
		Dst = v6;
		if ( v6 )
		{
			if ( *(_DWORD *)v11 )
			{
				v3 = *(_DWORD *)v11;
				v4 = CPacket::GetBuf(v11);
				_memmove(Dst, v4, v3);
			}
			if ( *((_DWORD *)v11 + 3) )
			{
				v7 = (void *)*((_DWORD *)v11 + 3);
				operator delete(v7);
			}
			*((_DWORD *)v11 + 3) = Dst;
			*((_DWORD *)v11 + 1) = v9;
			result = 1;
		}
		else
		{
			result = 0;
		}
	}
	else
	{
		result = 1;
	}
	return result;
}

int CPacket::CheckReceivePacketData()
{
	unsigned __int8 *v1; // eax@1
	unsigned __int16 v2; // ax@5
	unsigned __int16 v3; // ax@7
	char v5; // [sp+Ch] [bp-FCh]@1
	unsigned __int8 *v6; // [sp+D0h] [bp-38h]@1
	unsigned __int16 v7; // [sp+DCh] [bp-2Ch]@1
	unsigned __int8 *v8; // [sp+E8h] [bp-20h]@1
	int v9; // [sp+F4h] [bp-14h]@1
	CPacket *v10; // [sp+100h] [bp-8h]@1

	memset(&v5, 0xCCu, 0xFCu);
	v10 = this;
	v9 = 0;
	v8 = 0;
	v7 = 0;
	v6 = CPacket::GetData(this);
	v1 = CPacket::GetData(v10);
	if ( *((_DWORD *)v6 + 2) == *((_DWORD *)v1 + 2)
		&& *((_DWORD *)v6 + 2) <= 0xA00000u
		&& *((_DWORD *)v6 + 6) != *((_DWORD *)&g_globalData + 8) )
	{
		v7 = *((_WORD *)v6 + 6);
		*((_WORD *)v6 + 6) = 0;
		v2 = CRC16(v6, 0x2Cu);
		if ( v2 == v7 )
		{
			*((_WORD *)v6 + 6) = v7;
			v8 = CPacket::GetData(v10) + 44;
			v7 = *((_WORD *)v6 + 7);
			*((_WORD *)v6 + 7) = 0;
			v3 = CRC16(v8, *(_DWORD *)v8 + 4);
			if ( v3 == v7 )
			{
				*((_WORD *)v6 + 7) = v7;
				if ( ((*(_DWORD *)v8 + 15) & 0xFFFFFFF0) + 48 != *((_DWORD *)v6 + 2) )
					v9 = -536863740;
			}
			else
			{
				v9 = -536863740;
			}
		}
		else
		{
			v9 = -536863740;
		}
	}
	else
	{
		v9 = -536863742;
	}
	return v9;
}

int CPacket::TreatSendPacketData(unsigned __int32 a2, unsigned __int32 a3, unsigned __int32 a4, unsigned __int32 a5)
{
	int result; // eax@2
	char v6; // [sp+Ch] [bp-FCh]@1
	size_t Size; // [sp+D0h] [bp-38h]@12
	unsigned __int8 *v8; // [sp+DCh] [bp-2Ch]@12
	int v9; // [sp+E8h] [bp-20h]@10
	void *Dst; // [sp+F4h] [bp-14h]@8
	CPacket *v11; // [sp+100h] [bp-8h]@1

	memset(&v6, 0xCCu, 0xFCu);
	v11 = this;
	if ( *((DWORD *)&g_globalData + 49) )
	{
		if ( (WORD)a2
			&& ((a2 & 0xFFFF0000) == -65536
			|| (unsigned __int16)a2 == 0xFFFF
			|| (unsigned __int16)a2 != (*((DWORD *)&g_globalData + 8) & 0xFFFF)) )
		{
			Dst = CPacket::GetData(v11);
			if ( Dst )
			{
				v9 = *(_DWORD *)v11 - 48;
				memset(Dst, 0, 0x2Cu);
				*(_DWORD *)Dst = 44;
				*((_DWORD *)Dst + 1) = CPacket::GetPacketID(v11);
				*((_DWORD *)Dst + 2) = ((v9 + 15) & 0xFFFFFFF0) + 48;
				*((_DWORD *)Dst + 4) = a5;
				*((_DWORD *)Dst + 6) = *((_DWORD *)&g_globalData + 8);
				*((_DWORD *)Dst + 7) = a2;
				*((_DWORD *)Dst + 8) = a3;
				*((_DWORD *)Dst + 10) = a4;
				if ( *((_DWORD *)Dst + 2) <= 0xA00000u )
				{
					v8 = CPacket::GetData(v11) + 44;
					*(_DWORD *)v8 = v9;
					Size = ((v9 + 15) & 0xFFFFFFF0) - v9;
					if ( ((v9 + 15) & 0xFFFFFFF0) != v9 )
						memset(&v8[v9 + 4], 0, Size);
					v11->Encrypt();
					*((_WORD *)Dst + 7) = 0;
					*((_WORD *)Dst + 6) = 0;
					*((_WORD *)Dst + 7) = CRC16(v8, *(_DWORD *)v8 + 4);
					*((_WORD *)Dst + 6) = CRC16((unsigned __int8 *)Dst, 0x2Cu);
					result = 0;
				}
				else
				{
					result = -536863740;
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
	}
	else
	{
		result = -536863735;
	}
	return result;
}

int CPacket::TreatSendPacketData(unsigned __int32 a2)
{
	unsigned __int8 *v2; // eax@1
	char v4; // [sp+Ch] [bp-D8h]@1
	unsigned __int8 *v5; // [sp+D0h] [bp-14h]@1
	CPacket *v6; // [sp+DCh] [bp-8h]@1

	memset(&v4, 0xCCu, 0xD8u);
	v6 = this;
	v2 = GetData();
	v5 = v2;
	*((_DWORD *)v2 + 7) = a2;
	*((_WORD *)v5 + 6) = 0;
	*((_WORD *)v5 + 6) = CRC16(v5, 0x2Cu);
	return 0;
}

void CPacket::Encrypt()
{
	char v1; // [sp+Ch] [bp-E4h]@1
	unsigned __int8 *v2; // [sp+D0h] [bp-20h]@1
	unsigned __int8 *v3; // [sp+DCh] [bp-14h]@1
	CPacket *v4; // [sp+E8h] [bp-8h]@1

	memset(&v1, 0xCCu, 0xE4u);
	v4 = this;
	v3 = CPacket::GetData(this);
	v2 = CPacket::GetData(v4) + 44;
	if ( *((_DWORD *)v3 + 4) == 1 )
	{
		*((_DWORD *)v3 + 5) = CPacket::GetRandomDword(v4);
		CPacket::RandomTreatByDWORD(v4, v2 + 4, *(_DWORD *)v2, *((_DWORD *)v3 + 5));
	}
	else if ( *((_DWORD *)v3 + 4) == 2 )
	{
		CPacket::DesTreat(v4, v2 + 4, *(_DWORD *)v2, 1);
	}
}

#pragma once

class CPacket
{
public :
	CPacket();
	~CPacket();
	int AddBuf(const void *Src, unsigned __int32 a3);
	int AddData(const void *Src, unsigned __int32 Size);
	int CheckReceivePacketData();

	int TreatSendPacketData(unsigned __int32 a2, unsigned __int32 a3, unsigned __int32 a4, unsigned __int32 a5);
	int TreatSendPacketData(unsigned __int32 a2);
	void Encrypt();
private :
	DWORD dwArg0;
	DWORD dwArg1;
	DWORD dwArg2;
	DWORD dwArg3;
};

class CSafeCS
{
private:
	LPCRITICAL_SECTION m_pcs;

public:
	CSafeCS(LPCRITICAL_SECTION pcs);
};

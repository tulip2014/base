#pragma once

typedef struct tagICOM_CLIENTINFO
{
	DWORD dwFlag;										// 
	DWORD dwClientID;									// ClientID
	DWORD dwSessionID;									// ����Session��ID
	DWORD dwPID;										// ���ڽ��̵�PID
	DWORD dwUserNameID;									// �û�����ID
	TCHAR ClientPath[MAX_PATH];							// �ͻ�ģ�������·��
}ICOM_CLIENTINFO,*PICOM_CLIENTINFO;

typedef struct tagICOM_STARTSTOP
{
	DWORD m_dwSize;
	DWORD m_dwFlag;
	ICOM_CLIENTINFO ci;
	DWORD m_dwOriClientID;
	DWORD m_dwReserved;
}ICOM_STARTSTOP,*PICOM_STARTSTOP;

typedef struct tagCLIENTINFO
{
	DWORD	dwFlag;						// ��־λ
	HANDLE	hProcess;					// ���̾��
	ICOM_STARTSTOP ss;					// ģ����Ϣ�������ɹ����˳�ʱ�ᷢ�ʹ���Ϣ
}CLIENTINFO, *PCLIENTINFO;


int CalcClientID( PCLIENTINFO a1);
int SendPacketData(unsigned __int32 a1, unsigned __int32 a2, unsigned __int32 a3, struct CPacket *a4, struct CPacket *a5, unsigned __int32 a6);
int SendPacketData(CMyPipe *a1, unsigned __int32 a2, CPacket *a3, CPacket *a4);
int SendPacketData(struct CMyPipe *a1, struct CPacket *a2, struct CPacket *a3);
int SendPipePacket( CMyPipe *a1, CPacket *a2);
DWORD CreatePipeServerThread();
int SendBroadcastPacket(unsigned __int32 a1, CPacket *a2, CPacket *a3, unsigned __int32 a4, int *a5);
DWORD WINAPI PipeClientThread(LPVOID lpThreadParameter);
int TreatSendPacketData(CPacket *a1, unsigned __int32 a2);
DWORD WINAPI PipeServerThread(LPVOID lpThreadParameter);
int WaitClientLogIn(struct tagTHREADINFO *a1);
int ReceivePipePacket(struct CMyPipe *a1, struct CPacket *a2, int a3);
int ClientIDToPipe(unsigned __int32 *a1, struct CMyPipe **a2, unsigned __int32 *a3);
int ConnectToPipeServer(struct tagTHREADINFO *a1);


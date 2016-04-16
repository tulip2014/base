
#pragma once
#include <windows.h>

#define PCL_MAX_FUNC_NUM				5				// �ص������������

typedef struct tagPCL_MODULEINFO
{
	DWORD	m_dwSize;									// �ṹ��С�������ʼ��Ϊsizeof(PCL_MODULEINFO)
	DWORD	m_dwFlag;									// ��־λ����ʱû�ж��壬����Ϊ0
	DWORD	m_dwClientID;								// ClientID��������Ķ��壬ʹ��PCL_CalcSelfClientID����õ�
	DWORD	m_dwConnectTimeout;							// ���ӳ�ʱ(ms)�����Ϊ0��ʹ��Ĭ��ֵPCL_MAX_TIMEOUT
	DWORD	m_dwSendTimeout;							// ���ͳ�ʱ(ms)�����Ϊ0��ʹ��Ĭ��ֵPCL_MAX_TIMEOUT
	DWORD	m_dwRecvTimeout;							// ���ճ�ʱ(ms)�����Ϊ0��ʹ��Ĭ��ֵPCL_MAX_TIMEOUT
	DWORD	m_dwFuncNum;								// �ص���������
	LPVOID	m_FuncAddr[PCL_MAX_FUNC_NUM];				// �ص�����ָ��(��ʹ�õĺ���ָ������ʼ��ΪNULL)
}PCL_MODULEINFO, *PPCL_MODULEINFO;

// ���ֽṹ�Ķ���
typedef struct tagPCL_CLIENTINFO
{
	DWORD dwFlag;										// ��ʱû�ж��壬����Ϊ0
	DWORD dwClientID;									// ClientID��������Ķ��壬ʹ��PCL_CalcSelfClientID����õ�
	DWORD dwSessionID;									// ����Session��ID
	DWORD dwPID;										// ���ڽ��̵�PID
	DWORD dwUserNameID;									// �û�����ID
	TCHAR ClientPath[MAX_PATH];							// �ͻ�ģ�������·��
}PCL_CLIENTINFO, *PPCL_CLIENTINFO;

typedef struct tagCONFIGDATA
{
	REGCONFIGDATA		regdata;
	FILECONFIGDATA		filedata;
	DYNAMICCONFIGDATA	dynamicdata;
}CONFIGDATA,*PCONFIGDATA;

// ȫ�ֱ����Ķ��壬���е�ȫ�ֱ��������ڴ�
typedef struct tagGLOBALDATA
{
	DWORD	dwMagic;
	DWORD	dwOsVer;							// ����ϵͳ�汾
	DWORD	dwOsBuildNumber;					// Build
	DWORD	dwFlag;								// ȫ�ֱ�־λ
	DWORD	dwSoftState;						// ״̬��־λ
	CONFIGDATA ConfigData;						// ��������
	DWORD	dwInstallTime;						// ��װʱ��(time�����õ�)
	CMyThread* pFuncThread;						// �����߳�
	HANDLE	ControlEvent[MAX_CTRLEVENT];		// �����¼�
	DWORD	dwInitModFlag;						// ��ʼ�������־λ
	RUNTIMEDATA rd;								// ����ʱ����
	TCHAR	szBasePath[MAX_PATH];				// Base·������\��β
}GLOBALDATA,*PGLOBALDATA;

extern GLOBALDATA g_globalData;

int PCL_0001(PPCL_MODULEINFO *Src);
int PCL_0002();
void* PCL_0003(unsigned int a1);
void* PCL_0004(unsigned int a1, unsigned int a2);
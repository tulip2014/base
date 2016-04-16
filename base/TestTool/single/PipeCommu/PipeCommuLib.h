
#pragma once
#include <windows.h>

#define PCL_MAX_FUNC_NUM				5				// 回调函数最大数量

typedef struct tagPCL_MODULEINFO
{
	DWORD	m_dwSize;									// 结构大小，必须初始化为sizeof(PCL_MODULEINFO)
	DWORD	m_dwFlag;									// 标志位，暂时没有定义，必须为0
	DWORD	m_dwClientID;								// ClientID，见上面的定义，使用PCL_CalcSelfClientID计算得到
	DWORD	m_dwConnectTimeout;							// 连接超时(ms)，如果为0则使用默认值PCL_MAX_TIMEOUT
	DWORD	m_dwSendTimeout;							// 发送超时(ms)，如果为0则使用默认值PCL_MAX_TIMEOUT
	DWORD	m_dwRecvTimeout;							// 接收超时(ms)，如果为0则使用默认值PCL_MAX_TIMEOUT
	DWORD	m_dwFuncNum;								// 回调函数数量
	LPVOID	m_FuncAddr[PCL_MAX_FUNC_NUM];				// 回调函数指针(不使用的函数指针必须初始化为NULL)
}PCL_MODULEINFO, *PPCL_MODULEINFO;

// 各种结构的定义
typedef struct tagPCL_CLIENTINFO
{
	DWORD dwFlag;										// 暂时没有定义，必须为0
	DWORD dwClientID;									// ClientID，见上面的定义，使用PCL_CalcSelfClientID计算得到
	DWORD dwSessionID;									// 所在Session的ID
	DWORD dwPID;										// 所在进程的PID
	DWORD dwUserNameID;									// 用户名字ID
	TCHAR ClientPath[MAX_PATH];							// 客户模块自身的路径
}PCL_CLIENTINFO, *PPCL_CLIENTINFO;

typedef struct tagCONFIGDATA
{
	REGCONFIGDATA		regdata;
	FILECONFIGDATA		filedata;
	DYNAMICCONFIGDATA	dynamicdata;
}CONFIGDATA,*PCONFIGDATA;

// 全局变量的定义，所有的全局变量都放在此
typedef struct tagGLOBALDATA
{
	DWORD	dwMagic;
	DWORD	dwOsVer;							// 操作系统版本
	DWORD	dwOsBuildNumber;					// Build
	DWORD	dwFlag;								// 全局标志位
	DWORD	dwSoftState;						// 状态标志位
	CONFIGDATA ConfigData;						// 配置数据
	DWORD	dwInstallTime;						// 安装时间(time函数得到)
	CMyThread* pFuncThread;						// 功能线程
	HANDLE	ControlEvent[MAX_CTRLEVENT];		// 控制事件
	DWORD	dwInitModFlag;						// 初始化组件标志位
	RUNTIMEDATA rd;								// 运行时数据
	TCHAR	szBasePath[MAX_PATH];				// Base路径，以\结尾
}GLOBALDATA,*PGLOBALDATA;

extern GLOBALDATA g_globalData;

int PCL_0001(PPCL_MODULEINFO *Src);
int PCL_0002();
void* PCL_0003(unsigned int a1);
void* PCL_0004(unsigned int a1, unsigned int a2);
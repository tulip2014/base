

#include "windows.h"

#define __PROC_LOG_RELEASE__


	// 参数定义同PROC_InitProgramLog
	void PROC_DeleteProgramLog(LPCTSTR pProjectName=NULL,LPCTSTR pLogFilePath=NULL);

	// PROC_InitProgramLog函数的参数定义
	// DWORD dwMask的定义
#define PROC_LOG_MASK_ALL		0xFFFFFFFF

	// DWORD dwFlag的定义
#define PROC_LOG_DELETE_MASK		0x0000000F	// 控制日志的大小
#define PROC_LOG_DELETE_NO		0x00000000	// 不自动删除日志文件，dwMaxLogFileSize/dwMaxReserveSize参数无任何意义
#define PROC_LOG_DELETE_INIT		0x00000001	// 在初始化的时候根据文件大小来删除日志文件
#define PROC_LOG_DELETE_CLOSE	0x00000002	// 在关闭的时候根据文件大小来删除日志文件
#define PROC_LOG_DELETE_WRITE	0x00000004	// 在写日志的时候根据文件大小来删除日志文件

#define PROC_LOG_RELEASE			0x80000000	// Release版本也输出日志，默认只有Debug版本输出日志
#define PROC_LOG_SAFE			0x40000000	// 日志文件是否加密
#define PROC_LOG_PROJECTNAME		0x20000000	// 日志文件是否显示项目名称
#define PROC_LOG_THREADID		0x10000000	// 显示线程ID
#define PROC_LOG_TIME			0x08000000	// 日志文件是否显示时间戳

#ifdef __PROC_LOG_RELEASE__	
#define PROC_FLAG_INIT			(PROC_LOG_DELETE_WRITE | PROC_LOG_RELEASE | PROC_LOG_PROJECTNAME | PROC_LOG_THREADID | PROC_LOG_TIME | PROC_LOG_SAFE)
#else
#define PROC_FLAG_INIT			(PROC_LOG_DELETE_WRITE | PROC_LOG_RELEASE | PROC_LOG_PROJECTNAME | PROC_LOG_THREADID | PROC_LOG_TIME)
#endif

#define __PROC_LOG_ENABLE__

#ifdef __PROC_LOG_ENABLE__

	// DWORD dwMask 掩码，控制哪些日志需要真正写入
	// DWORD dwFlag 标志位
	// DWORD dwLogBufSize 日志缓冲区的大小，0为不使用缓冲，-1为使用默认缓冲大小(100K)，其它为实际缓冲区的大小
	// DWORD dwMaxLogFileSize 日志文件的最大尺寸(单位：KB)，0代表不自动删除日志文件
	// DWORD dwMaxReserveSize 日志文件的最大保留尺寸(单位：KB)，0代表全部删除，不保留
	// LPCTSTR pProjectName 项目名称，如果为NULL，取exe文件名
	// LPCTSTR pLogFilePath 日志文件路径，可以为绝对路径和相对路径，如果为相对路径，前面添加exe所在的目录，如果为NULL，则为exe所在的目录+log\\+pProjectName.log

	void PROC_InitProgramLog(DWORD dwMask=PROC_LOG_MASK_ALL,DWORD dwFlag=PROC_FLAG_INIT,DWORD dwLogBufSize=-1,DWORD dwMaxLogFileSize=0,DWORD dwMaxReserveSize=0,LPCTSTR pProjectName=NULL,LPCTSTR pLogFilePath=NULL);
	void PROC_InitProgramLogEx(DWORD dwMask=PROC_LOG_MASK_ALL,LPCTSTR pProjectName=NULL,LPCTSTR pLogFilePath=NULL);
	void PROC_InitProgramLogExByBuf(DWORD dwMask=PROC_LOG_MASK_ALL,LPCTSTR pProjectName=NULL,LPCTSTR pLogFilePath=NULL);

	void PROC_CloseProgramLog();

	void PROC_FlushProgramLog();	// 把缓存的日志写入文件

	void PROC_WriteProgramLogString(DWORD dwMask,LPCTSTR lpszLogText);
	void PROC_WriteProgramLogBin(DWORD dwMask,LPCTSTR lpszFront,LPCTSTR lpszBack,LPCTSTR lpszBuf,DWORD uBufLength);
	void PROC_WriteProgramLog(DWORD dwMask,LPCTSTR lpszFormat,...);
	void PROC_WriteProgramLogNoMask(LPCTSTR lpszFormat,...);

#define PROC_WriteProgramLogStringNoMask(lpszLogText)						PROC_WriteProgramLogString(PROC_LOG_MASK_ALL,lpszLogText)
#define PROC_WriteProgramLogBinNoMask(lpszFront,lpszBack,lpszBuf,nBufLength)	PROC_WriteProgramLogBin(PROC_LOG_MASK_ALL,lpszFront,lpszBack,lpszBuf,nBufLength)

#else	// __PROC_LOG_ENABLE__

#define PROC_InitProgramLog														__noop
#define PROC_InitProgramLogEx														__noop
#define PROC_InitProgramLogExNoBuf													__noop
#define PROC_CloseProgramLog()													__noop
#define PROC_FlushProgramLog()													__noop
#define PROC_WriteProgramLogString(dwMask,lpszLogText)							__noop
#define PROC_WriteProgramLogBin(dwMask,lpszFront,lpszBack,lpszBuf,nBufLength)	__noop
#define PROC_WriteProgramLog														__noop
#define PROC_WriteProgramLogNoMask												__noop

#define PROC_WriteProgramLogStringNoMask(lpszLogText)							__noop
#define PROC_WriteProgramLogBinNoMask(lpszFront,lpszBack,lpszBuf,nBufLength)		__noop

#endif	// __PROC_LOG_ENABLE__


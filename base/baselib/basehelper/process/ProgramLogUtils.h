

#include "windows.h"

#define __PROC_LOG_RELEASE__


	// ��������ͬPROC_InitProgramLog
	void PROC_DeleteProgramLog(LPCTSTR pProjectName=NULL,LPCTSTR pLogFilePath=NULL);

	// PROC_InitProgramLog�����Ĳ�������
	// DWORD dwMask�Ķ���
#define PROC_LOG_MASK_ALL		0xFFFFFFFF

	// DWORD dwFlag�Ķ���
#define PROC_LOG_DELETE_MASK		0x0000000F	// ������־�Ĵ�С
#define PROC_LOG_DELETE_NO		0x00000000	// ���Զ�ɾ����־�ļ���dwMaxLogFileSize/dwMaxReserveSize�������κ�����
#define PROC_LOG_DELETE_INIT		0x00000001	// �ڳ�ʼ����ʱ������ļ���С��ɾ����־�ļ�
#define PROC_LOG_DELETE_CLOSE	0x00000002	// �ڹرյ�ʱ������ļ���С��ɾ����־�ļ�
#define PROC_LOG_DELETE_WRITE	0x00000004	// ��д��־��ʱ������ļ���С��ɾ����־�ļ�

#define PROC_LOG_RELEASE			0x80000000	// Release�汾Ҳ�����־��Ĭ��ֻ��Debug�汾�����־
#define PROC_LOG_SAFE			0x40000000	// ��־�ļ��Ƿ����
#define PROC_LOG_PROJECTNAME		0x20000000	// ��־�ļ��Ƿ���ʾ��Ŀ����
#define PROC_LOG_THREADID		0x10000000	// ��ʾ�߳�ID
#define PROC_LOG_TIME			0x08000000	// ��־�ļ��Ƿ���ʾʱ���

#ifdef __PROC_LOG_RELEASE__	
#define PROC_FLAG_INIT			(PROC_LOG_DELETE_WRITE | PROC_LOG_RELEASE | PROC_LOG_PROJECTNAME | PROC_LOG_THREADID | PROC_LOG_TIME | PROC_LOG_SAFE)
#else
#define PROC_FLAG_INIT			(PROC_LOG_DELETE_WRITE | PROC_LOG_RELEASE | PROC_LOG_PROJECTNAME | PROC_LOG_THREADID | PROC_LOG_TIME)
#endif

#define __PROC_LOG_ENABLE__

#ifdef __PROC_LOG_ENABLE__

	// DWORD dwMask ���룬������Щ��־��Ҫ����д��
	// DWORD dwFlag ��־λ
	// DWORD dwLogBufSize ��־�������Ĵ�С��0Ϊ��ʹ�û��壬-1Ϊʹ��Ĭ�ϻ����С(100K)������Ϊʵ�ʻ������Ĵ�С
	// DWORD dwMaxLogFileSize ��־�ļ������ߴ�(��λ��KB)��0�����Զ�ɾ����־�ļ�
	// DWORD dwMaxReserveSize ��־�ļ���������ߴ�(��λ��KB)��0����ȫ��ɾ����������
	// LPCTSTR pProjectName ��Ŀ���ƣ����ΪNULL��ȡexe�ļ���
	// LPCTSTR pLogFilePath ��־�ļ�·��������Ϊ����·�������·�������Ϊ���·����ǰ�����exe���ڵ�Ŀ¼�����ΪNULL����Ϊexe���ڵ�Ŀ¼+log\\+pProjectName.log

	void PROC_InitProgramLog(DWORD dwMask=PROC_LOG_MASK_ALL,DWORD dwFlag=PROC_FLAG_INIT,DWORD dwLogBufSize=-1,DWORD dwMaxLogFileSize=0,DWORD dwMaxReserveSize=0,LPCTSTR pProjectName=NULL,LPCTSTR pLogFilePath=NULL);
	void PROC_InitProgramLogEx(DWORD dwMask=PROC_LOG_MASK_ALL,LPCTSTR pProjectName=NULL,LPCTSTR pLogFilePath=NULL);
	void PROC_InitProgramLogExByBuf(DWORD dwMask=PROC_LOG_MASK_ALL,LPCTSTR pProjectName=NULL,LPCTSTR pLogFilePath=NULL);

	void PROC_CloseProgramLog();

	void PROC_FlushProgramLog();	// �ѻ������־д���ļ�

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


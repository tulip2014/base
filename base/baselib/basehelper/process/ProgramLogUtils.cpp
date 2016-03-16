/********************************************************************
	created:	2016/03/16
	created:	16:3:2016   15:39
	filename: 	D:\work\tulip\base\baselib\basehelper\process\ProgramLogUtils.cpp
	file path:	D:\work\tulip\base\baselib\basehelper\process
	file base:	ProgramLogUtils
	file ext:	cpp
	author:		
	
	purpose:	
*********************************************************************/

#include "ProgramLogUtils.h"
#include <windows.h>
#include <TCHAR.h>
#include <stdio.h>
#include <time.h>
#include <io.h>
#include <stdlib.h>
#include <Strsafe.h>


#define FLAG_ISSET(flag,value)					(((flag) & (value))==(value))

#define MAX_LOGPATHLENGTH		520
#define MAX_LOG_BUFFER			(1024*100)
#define MAX_LOG_SINGLEBUFFER	(1024*10)

	// LPCTSTR pLogFilePath 日志文件路径，可以为绝对路径和相对路径，如果为相对路径，前面添加exe所在的目录，如果为NULL，则为exe所在的目录+pProjectName.log
	static void PROC_GetProgramLogPath(LPCTSTR pProjectName,LPCTSTR pLogFilePath,TCHAR* lpszPathOut)
	{
		if(pProjectName==NULL && pLogFilePath==NULL)
			return;

		TCHAR path_buffer[MAX_LOGPATHLENGTH];
		if(::GetModuleFileName(NULL,path_buffer,MAX_LOGPATHLENGTH)==0)
			return;

		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];
		_tsplitpath_s(path_buffer, drive, _MAX_DRIVE, dir, _MAX_DIR,
			fname, _MAX_FNAME, ext, _MAX_EXT);

		if(pLogFilePath==NULL)
		{
#ifdef __PROC_LOG_POS__
			_tcscpy_s(lpszPathOut,MAX_LOGPATHLENGTH,_T("C:\\"));
#else
			_tcscpy_s(lpszPathOut,MAX_LOGPATHLENGTH,drive);
			_tcscat_s(lpszPathOut,MAX_LOGPATHLENGTH,dir);
#endif
			_tcscat_s(lpszPathOut,MAX_LOGPATHLENGTH,_T("log\\"));
			_tcscat_s(lpszPathOut,MAX_LOGPATHLENGTH,pProjectName);
			_tcscat_s(lpszPathOut,MAX_LOGPATHLENGTH,_T(".LOG"));
		}
		else if(_tcslen(pLogFilePath)>=2 && (pLogFilePath[1]==_T(':') || (pLogFilePath[0]==_T('\\') && pLogFilePath[1]==_T('\\'))))
		{
			_tcscpy_s(lpszPathOut,MAX_LOGPATHLENGTH,pLogFilePath);
		}
		else
		{
			_tcscpy_s(lpszPathOut,MAX_LOGPATHLENGTH,drive);
			_tcscat_s(lpszPathOut,MAX_LOGPATHLENGTH,dir);
			_tcscat_s(lpszPathOut,MAX_LOGPATHLENGTH,pLogFilePath);
		}
	}

	void PROC_DeleteProgramLog(LPCTSTR pProjectName,LPCTSTR pLogFilePath)
	{
		if(pProjectName==NULL && pLogFilePath==NULL)
			return;

		TCHAR tcLogFilePath[MAX_LOGPATHLENGTH];
		PROC_GetProgramLogPath(pProjectName,pLogFilePath,tcLogFilePath);

		SetFileAttributes(tcLogFilePath,FILE_ATTRIBUTE_NORMAL);
		DeleteFile(tcLogFilePath);
	}

#ifdef __PROC_LOG_ENABLE__

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	class CPROC_SafeCS  
	{
		LPCRITICAL_SECTION pcs;
	public:
		CPROC_SafeCS(LPCRITICAL_SECTION lpcs)
		{
			pcs=lpcs;
			EnterCriticalSection(pcs);
		}

		~CPROC_SafeCS()
		{
			LeaveCriticalSection(pcs);
		}
	};

	typedef struct tagPROC_PROGRAMLOGPARAM
	{
		BOOL m_bInit;								// 是否正确初始化
		DWORD m_dwMask;								// 掩码
		DWORD m_dwFlag;								// 标志位
		DWORD m_dwMaxLogFileSize;					// 日志文件的最大尺寸(单位：KB)，0代表不自动删除日志文件
		DWORD m_dwMaxReserveSize;					// 日志文件的最大保留尺寸(单位：KB)，0代表全部删除，不保留
		DWORD m_dwInitTickCount;					// 初始时间
		CRITICAL_SECTION m_CS;						// 同步对象
		HANDLE m_hThread;							// 线程句柄
		HANDLE m_hExit;								// 退出事件
		TCHAR *m_Buffer;							// 缓冲区
		DWORD m_dwBufferPos;						// 缓冲区当前位置
		DWORD m_dwMaxBufferSize;					// 缓冲区最大值
		TCHAR m_strLogFilePath[MAX_LOGPATHLENGTH];	// 日志文件路径
		TCHAR m_strProjectName[MAX_LOGPATHLENGTH];	// 项目名称
	}PROC_PROGRAMLOGPARAM,*PPROC_PROGRAMLOGPARAM;

	static PROC_PROGRAMLOGPARAM m_GlobalParam={0};

#define PROC_LOG_Init				(m_GlobalParam.m_bInit)
#define PROC_LOG_dwMask			(m_GlobalParam.m_dwMask)
#define PROC_LOG_dwFlag			(m_GlobalParam.m_dwFlag)
#define PROC_LOG_dwMaxLogFileSize	(m_GlobalParam.m_dwMaxLogFileSize)
#define PROC_LOG_dwMaxReserveSize	(m_GlobalParam.m_dwMaxReserveSize)
#define PROC_LOG_dwInitTickCount	(m_GlobalParam.m_dwInitTickCount)
#define PROC_LOG_CS				(m_GlobalParam.m_CS)
#define PROC_LOG_hThread			(m_GlobalParam.m_hThread)
#define PROC_LOG_hExit				(m_GlobalParam.m_hExit)
#define PROC_LOG_Buffer			(m_GlobalParam.m_Buffer)
#define PROC_LOG_dwBufferPos		(m_GlobalParam.m_dwBufferPos)
#define PROC_LOG_dwMaxBufferSize	(m_GlobalParam.m_dwMaxBufferSize)
#define PROC_LOG_strLogFilePath	(m_GlobalParam.m_strLogFilePath)
#define PROC_LOG_strProjectName	(m_GlobalParam.m_strProjectName)

	void PROC_WriteLogFile(DWORD dwFlag,LPCTSTR lpszLog);
	DWORD WINAPI LogThread(LPVOID lpParam);

	DWORD WINAPI MyGetTickCount(double* pInterTime)
	{
		static DWORD m_dwTickCountOver=0;
		static DWORD m_dwLastTickCount=GetTickCount();

		DWORD m_dwCurrentTickCount=GetTickCount();
		if(m_dwCurrentTickCount<m_dwLastTickCount)
			m_dwTickCountOver++;

		if(pInterTime!=NULL)
		{
			double dInterTime=m_dwTickCountOver;
			dInterTime*=0x100000000;
			dInterTime+=m_dwCurrentTickCount;

			*pInterTime=(dInterTime-PROC_LOG_dwInitTickCount)/CLOCKS_PER_SEC;
		}

		m_dwLastTickCount=m_dwCurrentTickCount;

		return m_dwCurrentTickCount;
	}

	BOOL MyCreateDirectory(LPCTSTR lpszPath)
	{
		TCHAR tcPath[MAX_PATH];
		_tcscpy_s(tcPath,MAX_PATH,lpszPath);

		for(DWORD i=0;i<_tcslen(tcPath);i++)
		{
			if(tcPath[i]==_T('/'))
				tcPath[i]=_T('\\');
		}

		TCHAR *p=tcPath+3;

		while(TRUE)
		{
			p=_tcsstr(p,_T("\\"));

			if(p==NULL)
			{
//			CreateDirectory(strPath,NULL);
				break;
			}
			else
			{
				*p=_T('\0');
				CreateDirectory(tcPath,NULL);
				*p=_T('\\');
				p++;
			}
		}

		return TRUE;
	}

	void PROC_InitProgramLog(DWORD dwMask,DWORD dwFlag,DWORD dwLogBufSize,DWORD dwMaxLogFileSize,DWORD dwMaxReserveSize,LPCTSTR pProjectName,LPCTSTR pLogFilePath)
	{
		if(PROC_LOG_Init)
			return;

#if !defined(_DEBUG)
		if(!FLAG_ISSET(dwFlag,PROC_LOG_RELEASE))
		{
			ZeroMemory(&m_GlobalParam,sizeof(m_GlobalParam));
			PROC_LOG_Init=FALSE;
			return;
		}
#endif

		if(dwMaxReserveSize>=dwMaxLogFileSize)
			dwMaxReserveSize=0;

		TCHAR tcProgramFilePath[MAX_LOGPATHLENGTH]={0};
		if(::GetModuleFileName(NULL,tcProgramFilePath,MAX_LOGPATHLENGTH)==0)
			return;

		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];
		_tsplitpath_s(tcProgramFilePath,drive, _MAX_DRIVE, dir, _MAX_DIR, 
			fname, _MAX_FNAME, ext, _MAX_EXT);

		ZeroMemory(&m_GlobalParam,sizeof(m_GlobalParam));
		PROC_LOG_dwMask=dwMask;
		PROC_LOG_dwFlag=dwFlag;
		PROC_LOG_dwMaxLogFileSize=dwMaxLogFileSize;
		PROC_LOG_dwMaxReserveSize=dwMaxReserveSize;
		PROC_LOG_dwInitTickCount=MyGetTickCount(NULL);

		if(pProjectName==NULL)
			_tcscpy_s(PROC_LOG_strProjectName,MAX_LOGPATHLENGTH,fname);
		else
			_tcscpy_s(PROC_LOG_strProjectName,MAX_LOGPATHLENGTH,pProjectName);

		PROC_GetProgramLogPath(PROC_LOG_strProjectName,pLogFilePath,PROC_LOG_strLogFilePath);

		MyCreateDirectory(PROC_LOG_strLogFilePath);

		InitializeCriticalSection(&PROC_LOG_CS);

		if(dwLogBufSize==0)
		{
			PROC_LOG_Buffer=NULL;
			PROC_LOG_dwBufferPos=0;
			PROC_LOG_dwMaxBufferSize=0;
		}
		else
		{
			if(dwLogBufSize==(DWORD)-1)
				PROC_LOG_dwMaxBufferSize=MAX_LOG_BUFFER;
			else
			{
				PROC_LOG_dwMaxBufferSize=dwLogBufSize;
				if(PROC_LOG_dwMaxBufferSize>10*1024*1024)
					PROC_LOG_dwMaxBufferSize=10*1024*1024;
			}
			PROC_LOG_Buffer=new TCHAR[PROC_LOG_dwMaxBufferSize];
			PROC_LOG_dwBufferPos=0;
			if(PROC_LOG_Buffer==NULL)
				return;

			PROC_LOG_hExit=CreateEvent(NULL,FALSE,FALSE,NULL);
			if(PROC_LOG_hExit==NULL)
				return;

			DWORD dwThreadID=0;
			PROC_LOG_hThread=CreateThread(NULL,0,LogThread,PROC_LOG_hExit,0,&dwThreadID);
			if(PROC_LOG_hThread==NULL)
				return;
		}

		PROC_LOG_Init=TRUE;

		TCHAR tcBuf[MAX_LOG_SINGLEBUFFER] = {0};
		size_t len = _tcslen(tcBuf);
		_stprintf_s(tcBuf, (MAX_LOG_SINGLEBUFFER-len), _T("\r\n\r\n"));

		if(FLAG_ISSET(PROC_LOG_dwFlag,PROC_LOG_PROJECTNAME))
			_stprintf_s(tcBuf+_tcslen(tcBuf), (MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)), _T("Project Name: %s\r\n"),PROC_LOG_strProjectName);

		_stprintf_s(tcBuf+_tcslen(tcBuf), (MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)), _T("Project Path<%u.%u>: %s\r\n"),GetCurrentProcessId(),GetCurrentThreadId(),tcProgramFilePath);

		if(FLAG_ISSET(PROC_LOG_dwFlag,PROC_LOG_TIME))
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			_stprintf_s(tcBuf+_tcslen(tcBuf), (MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)), _T("Start Time: %10u %04d-%02d-%02d %02d:%02d:%02d\r\n"),PROC_LOG_dwInitTickCount,
				st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
		}

		_stprintf_s(tcBuf+_tcslen(tcBuf), (MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)), _T("\r\n"));

		PROC_WriteLogFile(PROC_LOG_DELETE_INIT | PROC_LOG_DELETE_WRITE,tcBuf);
	}

	void PROC_InitProgramLogEx(DWORD dwMask,LPCTSTR pProjectName,LPCTSTR pLogFilePath)
	{
		PROC_InitProgramLog(dwMask,PROC_FLAG_INIT,0,5*1024*1024,2*1024*1024,pProjectName,pLogFilePath);
	}

	void PROC_InitProgramLogExByBuf(DWORD dwMask,LPCTSTR pProjectName,LPCTSTR pLogFilePath)
	{
		PROC_InitProgramLog(dwMask,PROC_FLAG_INIT,100*1024,5*1024*1024,2*1024*1024,pProjectName,pLogFilePath);
	}

	void PROC_CloseProgramLog()
	{
		if(!PROC_LOG_Init)
			return;

		if(PROC_LOG_hThread!=NULL)
		{
			if(PROC_LOG_hExit!=NULL)
			{
				SetEvent(PROC_LOG_hExit);
				if(WaitForSingleObject(PROC_LOG_hThread,1000)==WAIT_TIMEOUT)
					TerminateThread(PROC_LOG_hThread,0);

				CloseHandle(PROC_LOG_hExit);
				PROC_LOG_hExit=NULL;
			}
			else
				TerminateThread(PROC_LOG_hThread,0);

			CloseHandle(PROC_LOG_hThread);
			PROC_LOG_hThread=NULL;
		}

		if(PROC_LOG_Buffer!=NULL)
		{
			delete[] PROC_LOG_Buffer;
			PROC_LOG_Buffer=NULL;
		}
		PROC_LOG_dwBufferPos=0;
		PROC_LOG_dwMaxLogFileSize=0;
		PROC_LOG_dwMaxReserveSize=0;

		TCHAR tcProgramFilePath[MAX_LOGPATHLENGTH]={0};
		if(::GetModuleFileName(NULL,tcProgramFilePath,MAX_LOGPATHLENGTH)==0)
			return;

		TCHAR tcBuf[MAX_LOG_SINGLEBUFFER] = {0};

		if(FLAG_ISSET(PROC_LOG_dwFlag,PROC_LOG_PROJECTNAME))
			_stprintf_s(tcBuf+_tcslen(tcBuf), (MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)), _T("\r\nProject Name: %s\r\n"),PROC_LOG_strProjectName);

		_stprintf_s(tcBuf+_tcslen(tcBuf), (MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)), _T("Project Path<%u.%u>: %s\r\n"),GetCurrentProcessId(),GetCurrentThreadId(),tcProgramFilePath);

		if(FLAG_ISSET(PROC_LOG_dwFlag,PROC_LOG_TIME))
		{
			SYSTEMTIME st;
			GetLocalTime(&st);
			_stprintf_s(tcBuf+_tcslen(tcBuf),(MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)),_T("End Time: %10u %04d-%02d-%02d %02d:%02d:%02d\r\n"),MyGetTickCount(NULL),
				st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
		}

		PROC_WriteLogFile(PROC_LOG_DELETE_CLOSE | PROC_LOG_DELETE_WRITE,tcBuf);

		PROC_LOG_Init=FALSE;

		DeleteCriticalSection(&PROC_LOG_CS);
	}

	void PROC_FlushProgramLog()	// 把缓存的日志写入文件
	{
//		if(!PROC_LOG_Init)
//			return;

//		PROC_WriteLogFile(PROC_LOG_DELETE_WRITE,PROC_LOG_Buffer);
//		PROC_LOG_dwBufferPos=0;
	}

	void PROC_WriteProgramLogString(DWORD dwMask,LPCTSTR lpszLogText)
	{
		if(!PROC_LOG_Init)
			return;

		if((PROC_LOG_dwMask & dwMask)==0)
			return;

		TCHAR tcBuf[MAX_LOG_SINGLEBUFFER] = {0};
		//tcBuf[0]=_T('\0');

		HRESULT hr(E_FAIL);

		if(FLAG_ISSET(PROC_LOG_dwFlag,PROC_LOG_TIME))
		{
			double dInterTime=0;
			DWORD dwTickCount=MyGetTickCount(&dInterTime);
			hr = StringCchPrintf(tcBuf+_tcslen(tcBuf),(MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)),_T("%10u %10.3f "),dwTickCount,dInterTime);
		}

		if(FLAG_ISSET(PROC_LOG_dwFlag,PROC_LOG_PROJECTNAME) && FLAG_ISSET(PROC_LOG_dwFlag,PROC_LOG_TIME))
		{
			if(FLAG_ISSET(PROC_LOG_dwFlag,PROC_LOG_THREADID))
				hr = StringCchPrintf(tcBuf+_tcslen(tcBuf),(MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)),_T("%s<%u>:: "),PROC_LOG_strProjectName,GetCurrentThreadId());
			else
				hr = StringCchPrintf(tcBuf+_tcslen(tcBuf),(MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)),_T("%s:: "),PROC_LOG_strProjectName);
		}

		hr = StringCchPrintf(tcBuf+_tcslen(tcBuf),(MAX_LOG_SINGLEBUFFER-_tcslen(tcBuf)),_T("%s\r\n"),lpszLogText);
		if(SUCCEEDED(hr))
			PROC_WriteLogFile(PROC_LOG_DELETE_WRITE,tcBuf);
/*
		if(PROC_LOG_Buffer!=NULL)
		{
			if(PROC_LOG_dwBufferPos+_tcslen(tcBuf)<PROC_LOG_dwMaxBufferSize)
			{
				memmove(PROC_LOG_Buffer+PROC_LOG_dwBufferPos,tcBuf,_tcslen(tcBuf)*sizeof(TCHAR));
				PROC_LOG_dwBufferPos+=_tcslen(tcBuf);
				PROC_LOG_Buffer[PROC_LOG_dwBufferPos]=_T('\0');
			}
			else
			{
				PROC_WriteLogFile(PROC_LOG_DELETE_WRITE,PROC_LOG_Buffer);
				PROC_LOG_dwBufferPos=0;

				PROC_WriteLogFile(PROC_LOG_DELETE_WRITE,tcBuf);
			}
		}
		else
			PROC_WriteLogFile(PROC_LOG_DELETE_WRITE,tcBuf);
*/
	}

	inline TCHAR Hex(char x)
	{
		if(x>=0 && x<=9)
			return x+_T('0');
		else if(x>=10 && x<=16)
			return x-10+_T('A');

		return _T(' ');
	}

	void PROC_WriteProgramLogBin(DWORD dwMask,LPCTSTR lpszFront,LPCTSTR lpszBack,LPCTSTR lpszBuf,DWORD uBufLength)
	{
		if(!PROC_LOG_Init)
			return;

		if((PROC_LOG_dwMask & dwMask)==0)
			return;

		TCHAR tcBuf[MAX_LOG_SINGLEBUFFER];
		tcBuf[0]=_T('\0');

		if(lpszFront!=NULL)
		{
			_tcscat_s(tcBuf,MAX_LOG_SINGLEBUFFER,lpszFront);
		}

		if((lpszBuf!=NULL) && (uBufLength>0))
		{
			TCHAR *pText;

			DWORD uNewBufLen;
#ifdef _UNICODE
			uNewBufLen=uBufLength*5;
#else
			uNewBufLen=uBufLength*3;
#endif	// _UNICODE

			pText=(TCHAR*)malloc(uNewBufLen*sizeof(TCHAR));
			if(pText==NULL)
				return;

			DWORD uPos;
			for(DWORD i=0;i<uBufLength;i++)
			{
#ifdef _UNICODE
				uPos=i*5;
				pText[uPos+0]=Hex((lpszBuf[i]>>12) & 0x0F);
				pText[uPos+1]=Hex((lpszBuf[i]>>8) & 0x0F);
				pText[uPos+2]=Hex((lpszBuf[i]>>4) & 0x0F);
				pText[uPos+3]=Hex(lpszBuf[i] & 0x0F);
				pText[uPos+4]=_T(' ');
#else
				uPos=i*3;
				pText[uPos+0]=Hex((lpszBuf[i]>>4) & 0x0F);
				pText[uPos+1]=Hex(lpszBuf[i] & 0x0F);
				pText[uPos+2]=_T(' ');
#endif	// _UNICODE
			}

#ifdef _UNICODE
			pText[uPos+4]=_T('\0');
#else
			pText[uPos+2]=_T('\0');
#endif	// _UNICODE

			_tcscat_s(tcBuf,MAX_LOG_SINGLEBUFFER,pText);

			free(pText);
		}

		if(lpszBack!=NULL)
		{
			_tcscat_s(tcBuf,MAX_LOG_SINGLEBUFFER,lpszBack);
		}

		PROC_WriteProgramLogString(dwMask,tcBuf);
	}

	void PROC_WriteProgramLog(DWORD dwMask,LPCTSTR lpszFormat,...)
	{
		if(!PROC_LOG_Init)
			return;

		if((PROC_LOG_dwMask & dwMask)==0)
			return;

		TCHAR tcBuf[MAX_LOG_SINGLEBUFFER];

		va_list arglist;
		va_start(arglist,lpszFormat);

		HRESULT hr = StringCchVPrintf(tcBuf,MAX_LOG_SINGLEBUFFER,lpszFormat,arglist);
		if(SUCCEEDED(hr))
			PROC_WriteProgramLogString(dwMask,tcBuf);
	}

	void PROC_WriteProgramLogNoMask(LPCTSTR lpszFormat,...)
	{
		if(!PROC_LOG_Init)
			return;

		TCHAR tcBuf[MAX_LOG_SINGLEBUFFER];

		va_list arglist;
		va_start(arglist,lpszFormat);

		HRESULT hr = StringCchVPrintf(tcBuf,MAX_LOG_SINGLEBUFFER,lpszFormat,arglist);
		if(SUCCEEDED(hr))
			PROC_WriteProgramLogString(-1,tcBuf);
	}

// 多线程写日志的代码
	static void PROC_WriteFile(HANDLE hFile,LPVOID pBuf,DWORD dwLength)
	{
		DWORD dwRet=0;
		WriteFile(hFile,pBuf,dwLength,&dwRet,NULL);
	}

	static void PROC_SafeWriteFile(HANDLE hFile,LPVOID pBuf,DWORD dwLength)
	{
		LPBYTE p=(LPBYTE)pBuf;
		BYTE bSeed=0x5A;

		for(DWORD i=0;i<dwLength;i++)
		{
			if(p[i]!=0 && p[i]!=bSeed)
				p[i]^=bSeed;
		}

		PROC_WriteFile(hFile,p,dwLength);
	}

	static void PROC_WriteFileBuf(HANDLE hFile,LPVOID pBuf,DWORD dwBufLength)
	{
		if(FLAG_ISSET(PROC_LOG_dwFlag,PROC_LOG_SAFE))
			PROC_SafeWriteFile(hFile,pBuf,dwBufLength);
		else
			PROC_WriteFile(hFile,pBuf,dwBufLength);
	}

	static void PROC_TruncLogFile(HANDLE hFile)
	{
		DWORD dwFileSize=GetFileSize(hFile,NULL);

		if(PROC_LOG_dwMaxLogFileSize!=0 && dwFileSize>PROC_LOG_dwMaxLogFileSize)
		{
			// 文件大小满足要求
			if(PROC_LOG_dwMaxReserveSize>0 && PROC_LOG_dwMaxReserveSize<PROC_LOG_dwMaxLogFileSize)
			{
				// 保留部分数据
				SetFilePointer(hFile,dwFileSize-PROC_LOG_dwMaxReserveSize,0,FILE_BEGIN);

				BYTE *pBuf=new BYTE[PROC_LOG_dwMaxReserveSize];
				if(pBuf==NULL)
				{
					SetFilePointer(hFile,0,0,FILE_END);
					return;
				}

				DWORD dwRead=0;
				DWORD dwRet;

				while(dwRead<PROC_LOG_dwMaxReserveSize)
				{
					if(!ReadFile(hFile,pBuf+dwRead,PROC_LOG_dwMaxReserveSize-dwRead,&dwRet,NULL))
						break;
					if(dwRet>=0)
						dwRead+=dwRet;
				}

				if(dwRead<PROC_LOG_dwMaxReserveSize)
				{
					delete[] pBuf;
					SetFilePointer(hFile,0,0,FILE_END);
					return;
				}

				SetFilePointer(hFile,0,0,FILE_BEGIN);

#ifdef _UNICODE
				WORD format=0xFEFF;			// little-endian UNICODE
				PROC_WriteFileBuf(hFile,(LPBYTE)&format,sizeof(WORD));
#endif	// _UNICODE

				WriteFile(hFile,pBuf,PROC_LOG_dwMaxReserveSize,&dwRet,NULL);

				delete[] pBuf;
			}
			else
			{
				SetFilePointer(hFile,0,0,FILE_BEGIN);

#ifdef _UNICODE
				WORD format=0xFEFF;			// little-endian UNICODE
				PROC_WriteFileBuf(hFile,(LPBYTE)&format,sizeof(WORD));
#endif	// _UNICODE
			}

			SetEndOfFile(hFile);
		}
	}

	static void PROC_OriWriteLogFile(DWORD dwFlag,LPCTSTR lpszLog,DWORD dwLength)
	{
		if(!PROC_LOG_Init)
			return;

		SetFileAttributes(PROC_LOG_strLogFilePath,FILE_ATTRIBUTE_NORMAL);

		HANDLE hFile=CreateFile(PROC_LOG_strLogFilePath,GENERIC_WRITE | GENERIC_READ,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile==INVALID_HANDLE_VALUE)
			return;

		if((dwFlag & PROC_LOG_dwFlag & PROC_LOG_DELETE_INIT)==PROC_LOG_DELETE_INIT ||
			(dwFlag & PROC_LOG_dwFlag & PROC_LOG_DELETE_CLOSE)==PROC_LOG_DELETE_CLOSE ||
			(dwFlag & PROC_LOG_dwFlag & PROC_LOG_DELETE_WRITE)==PROC_LOG_DELETE_WRITE)
		{
			PROC_TruncLogFile(hFile);
		}

		SetFilePointer(hFile,0,0,FILE_END);

#ifdef _UNICODE
		DWORD dwFileSize=GetFileSize(hFile,NULL);
		if(dwFileSize==0)
		{
			WORD format=0xFEFF;			// little-endian UNICODE
			PROC_WriteFileBuf(hFile,(LPBYTE)&format,sizeof(WORD));
		}
#endif	// _UNICODE

		PROC_WriteFileBuf(hFile,(LPBYTE)lpszLog,dwLength*sizeof(TCHAR));

		CloseHandle(hFile);
	}

	static void PROC_WriteLogFile(DWORD dwFlag,LPCTSTR lpszLog)
	{
		if(!PROC_LOG_Init)
			return;

		CPROC_SafeCS msc(&PROC_LOG_CS);

		if(PROC_LOG_hThread==NULL || PROC_LOG_Buffer==NULL || PROC_LOG_dwMaxBufferSize==0 || FLAG_ISSET(dwFlag,PROC_LOG_DELETE_INIT))
			PROC_OriWriteLogFile(dwFlag,lpszLog,_tcslen(lpszLog));
		else
		{
			DWORD dwLogLength=_tcslen(lpszLog);
			if(PROC_LOG_dwBufferPos+dwLogLength+1<PROC_LOG_dwMaxBufferSize)
			{
				_tcscpy_s(PROC_LOG_Buffer+PROC_LOG_dwBufferPos,PROC_LOG_dwMaxBufferSize-PROC_LOG_dwBufferPos,lpszLog);
				PROC_LOG_dwBufferPos+=dwLogLength;
			}
		}
	}

	static DWORD WINAPI LogThread(LPVOID lpParam)
	{
		HANDLE hExit=(HANDLE)lpParam;
		if(hExit==NULL)
			return 0;

		while(TRUE)
		{
			DWORD dwWait=WaitForSingleObject(hExit,500);

			switch(dwWait)
			{
			case WAIT_OBJECT_0+0:
			case WAIT_TIMEOUT:
				{
					TCHAR *pBuf=NULL;
					DWORD dwLength=0;

					{
						CPROC_SafeCS msc(&PROC_LOG_CS);

						if(PROC_LOG_dwBufferPos>0)
						{
							dwLength=PROC_LOG_dwBufferPos;
							pBuf=new TCHAR[dwLength+1];
							if(pBuf!=NULL)
							{
								memmove(pBuf,PROC_LOG_Buffer,(dwLength+1)*sizeof(TCHAR));
								PROC_LOG_dwBufferPos=0;
							}
						}
					}

					if(pBuf!=NULL && dwLength>0)
					{
						PROC_OriWriteLogFile(PROC_LOG_DELETE_WRITE,pBuf,dwLength);

						delete[] pBuf;
						pBuf=NULL;
					}

					if(dwWait==WAIT_OBJECT_0+0)
						goto ENDPOS;
				}
				break;
			}
		}

ENDPOS:

		return 0;
	}

#endif	// __PROC_LOG_ENABLE__



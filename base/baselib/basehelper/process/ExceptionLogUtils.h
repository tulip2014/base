/********************************************************************
	created:	2016/03/16
	created:	16:3:2016   15:06
	filename: 	base\baselib\basehelper\process\ExceptionLogUtils.h
	file path:	base\baselib\basehelper\process
	file base:	ExceptionLogUtils
	file ext:	h
	author:		murisly
	
	purpose:	
*********************************************************************/

#pragma once


#include <windows.h>

namespace ExceptionLogUtils{

	typedef void (__stdcall *expCallBack)(LPWSTR lpDumpPath);

	// ��ʼ���쳣ת������(���̳�ʼ����ʱ����ã�dll����Ҫ����)
	// DWORD dwFlag��Ԥ���ı�־λ����ʱȫ�����Ϊ0
	// LPCTSTR lpszVersion���汾
	// LPCTSTR lpszLogFilePath����־�ļ���·����ΪNULL�Ļ������ǽ�����ģ�����ڵ�·����LogĿ¼��
	// ��־�ļ�������ɹ�����Ŀ����+�汾+����+ʱ��.dmp������eSafeSvc_1.0.10582.47_201304080312090012.dmp
	bool InitialExceptionLog(unsigned int dwFlag, LPCTSTR lpszProjectName, LPCTSTR lpszVersion, LPCTSTR lpszLogFilePath = NULL, expCallBack farProc = NULL);
	// ����ͬ��������������һ���ṩ�����Ľӿ�
	bool InitialExceptionLogEx(unsigned int dwFlag, LPCTSTR lpszProjectName, LPCTSTR lpszVersion, LPCTSTR lpszLogFilePath = NULL, expCallBack farProc = NULL, LPVOID pHookFunc = NULL);

	// �����쳣ת������(�����˳�ǰ����)
	bool ClearExceptionLog();
}




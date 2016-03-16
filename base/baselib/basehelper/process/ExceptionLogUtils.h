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

	// 初始化异常转储功能(进程初始化的时候调用，dll不需要调用)
	// DWORD dwFlag：预留的标志位，暂时全部填充为0
	// LPCTSTR lpszVersion：版本
	// LPCTSTR lpszLogFilePath：日志文件的路径，为NULL的话，则是进程主模块所在的路径的Log目录下
	// 日志文件名的组成规则：项目名称+版本+日期+时间.dmp，比如eSafeSvc_1.0.10582.47_201304080312090012.dmp
	bool InitialExceptionLog(unsigned int dwFlag, LPCTSTR lpszProjectName, LPCTSTR lpszVersion, LPCTSTR lpszLogFilePath = NULL, expCallBack farProc = NULL);
	// 功能同上述函数，增加一个提供函数的接口
	bool InitialExceptionLogEx(unsigned int dwFlag, LPCTSTR lpszProjectName, LPCTSTR lpszVersion, LPCTSTR lpszLogFilePath = NULL, expCallBack farProc = NULL, LPVOID pHookFunc = NULL);

	// 清理异常转储功能(进程退出前调用)
	bool ClearExceptionLog();
}




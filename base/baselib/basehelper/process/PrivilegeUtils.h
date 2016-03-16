/********************************************************************
	created:	2016/03/17
	created:	17:3:2016   0:02
	filename: 	E:\work\git\base\base\base\baselib\basehelper\process\PrivilegeUtils.h
	file path:	E:\work\git\base\base\base\baselib\basehelper\process
	file base:	PrivilegeUtils
	file ext:	h
	author:		
	version:    1.0.0
	purpose:	
*********************************************************************/

#pragma once


#include <windows.h>


namespace privilege{

	// 判断当前用户是不是超级管理员
	BOOL IsUserAnAdmin();

	// 给当前进程增加SE_DEBUG_NAME权限，用来杀进程(仅限于超级管理员进程调用)
	BOOL WINAPI EnableProcessDebugPrivilege();

}

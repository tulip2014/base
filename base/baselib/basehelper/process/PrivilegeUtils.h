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

	// �жϵ�ǰ�û��ǲ��ǳ�������Ա
	BOOL IsUserAnAdmin();

	// ����ǰ��������SE_DEBUG_NAMEȨ�ޣ�����ɱ����(�����ڳ�������Ա���̵���)
	BOOL WINAPI EnableProcessDebugPrivilege();

}

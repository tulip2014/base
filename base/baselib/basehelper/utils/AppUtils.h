/********************************************************************
	created:	2016/03/10
	created:	10:3:2016   16:42
	filename: 	base\baselib\basehelper\utils\AppUtils.h
	file path:	base\baselib\basehelper\utils
	file base:	AppUtils
	file ext:	h
	author:		murisly
	
	purpose:	a module to get app info
*********************************************************************/

#pragma once

#include "..\common\Singleton.h"


class CAppUtils : public SingletonStatic<CAppUtils>
{
public:
	CAppUtils();
	~CAppUtils();


};

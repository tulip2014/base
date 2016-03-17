
/********************************************************************
	created:	2016/03/17
	created:	17:3:2016   21:48
	filename: 	base\baselib\basehelper\safe\cryptopp\md5.h
	file path:	base\baselib\basehelper\safe\cryptopp
	file base:	md5
	file ext:	h
	author:		
	version:    1.0.0
	purpose:	
*********************************************************************/

#pragma once

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "md5.h"
#include <tchar.h>
#include <windows.h>

namespace safe { namespace crypt {
	std::wstring GetMD5W(LPCWSTR lpStr);
	std::string GetMD5A(LPCSTR lpStr);
}}
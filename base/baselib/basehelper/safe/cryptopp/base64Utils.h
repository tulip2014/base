
/********************************************************************
	created:	2016/03/17
	created:	17:3:2016   23:19
	filename: 	baselib\basehelper\safe\cryptopp\base64Utils.h
	file path:	baselib\basehelper\safe\cryptopp
	file base:	base64Utils
	file ext:	h
	author:		
	version:    1.0.0
	purpose:	
*********************************************************************/

#pragma once

#include "base64.h"
#include <windows.h>

namespace safe { namespace crypt {
	std::wstring Base64EncodeW(LPCWSTR lpStr);
}}
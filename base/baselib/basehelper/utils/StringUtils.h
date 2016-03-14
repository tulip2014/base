
/********************************************************************
	created:	2016/03/14
	created:	14:3:2016   11:34
	filename: 	base\baselib\basehelper\StringUtils.h
	file path:	base\baselib\basehelper
	file base:	StringUtils
	file ext:	h
	author:		murisly
	
	purpose:	string helper
*********************************************************************/

#pragma once

#include <string>


namespace StringUtils {

	std::string  ws2s(const std::wstring& s_src, UINT CodePage = CP_UTF8);
	std::wstring s2ws( const std::string& s_src, UINT CodePage = CP_UTF8);

	std::wstring to_lower(const std::wstring& in);
	std::string to_lower(const std::string& in);

	std::wstring to_upper(const std::wstring& in);
	std::string to_upper(const std::string& in);

}
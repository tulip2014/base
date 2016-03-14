
/********************************************************************
	created:	2016/03/14
	created:	14:3:2016   11:39
	filename: 	base\baselib\basehelper\StringUtils.cpp
	file path:	base\baselib\basehelper
	file base:	StringUtils
	file ext:	cpp
	author:		murisly
	
	purpose:	
*********************************************************************/

#include "StringUtils.h"

namespace StringUtils{

	std::string ws2s(const std::wstring& wide, UINT CodePage)
	{
		int wide_length = static_cast<int>(wide.length());
		if (wide_length == 0)
			return std::string();

		// Compute the length of the buffer we'll need.
		int charcount = WideCharToMultiByte(CodePage, 0, wide.data(), wide_length,
			NULL, 0, NULL, NULL);
		if (charcount == 0)
			return std::string();

		std::string mb;
		mb.resize(charcount);
		WideCharToMultiByte(CodePage, 0, wide.data(), wide_length,
			&mb[0], charcount, NULL, NULL);

		return mb;
	}

	std::wstring s2ws( const std::string& mb,UINT CodePage)
	{
		if (mb.empty())
			return std::wstring();

		int mb_length = static_cast<int>(mb.length());
		// Compute the length of the buffer.
		int charcount = MultiByteToWideChar(CodePage, 0,
			mb.data(), mb_length, NULL, 0);
		if (charcount == 0)
			return std::wstring();

		std::wstring wide;
		wide.resize(charcount);
		MultiByteToWideChar(CodePage, 0, mb.data(), mb_length, &wide[0], charcount);

		return wide;
	}

	std::wstring to_lower( const std::wstring& in )
	{
		std::wstring out;
		out.resize(in.size());
		for (size_t i = 0; i<in.size(); i++)
		{
			out[i] = (wchar_t)towlower(in[i]);
		}
		return out;
	}

	std::string to_lower( const std::string& in )
	{
		std::string out;
		out.resize(in.size());
		for (size_t i = 0; i<in.size(); i++)
		{
			out[i] = (char)tolower(in[i]);
		}
		return out;
	}

	std::wstring to_upper( const std::wstring& in )
	{
		std::wstring out;
		out.resize(in.size());
		for (size_t i = 0; i<in.size(); i++)
		{
			out[i] = (wchar_t)toupper(in[i]);
		}
		return out;
	}

	std::string to_upper( const std::string& in )
	{
		std::string out;
		out.resize(in.size());
		for (size_t i = 0; i<in.size(); i++)
		{
			out[i] = (char)toupper(in[i]);
		}
		return out;
	}
}
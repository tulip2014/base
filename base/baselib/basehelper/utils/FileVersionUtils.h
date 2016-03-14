
/********************************************************************
	created:	2016/03/14
	created:	14:3:2016   15:00
	filename: 	base\baselib\basehelper\utils\FileVersionUtils.h
	file path:	base\baselib\basehelper\utils
	file base:	FileVersionUtils
	file ext:	h
	author:		murisly
	
	purpose:	to get file attribute
*********************************************************************/

#pragma once


#include <windows.h>
#include <string>

class CFileVersionUtils
{
public:
	~CFileVersionUtils();

	static CFileVersionUtils* CreateCFileVersionUtils(const std::wstring& file_path);

	// Accessors to the different version properties.
	// Returns an empty string if the property is not found.
	std::wstring company_name();
	std::wstring company_short_name();
	std::wstring product_name();
	std::wstring product_short_name();
	std::wstring internal_name();
	std::wstring product_version();
	std::wstring private_build();
	std::wstring special_build();
	std::wstring comments();
	std::wstring original_filename();
	std::wstring file_description();
	std::wstring file_version();
	std::wstring legal_copyright();
	std::wstring legal_trademarks();
	std::wstring last_change();
	bool is_official_build();

	// Lets you access other properties not covered above.
	bool GetValue(const wchar_t* name, std::wstring* value);

	// Similar to GetValue but returns a wstring (empty string if the property
	// does not exist).
	std::wstring GetStringValue(const wchar_t* name);

	// Get the fixed file info if it exists. Otherwise NULL
	VS_FIXEDFILEINFO* fixed_file_info() { return fixed_file_info_; }

private:
	CFileVersionUtils(void* data, int language, int code_page);
	//malloc
	char* data_;
	int language_;
	int code_page_;
	// This is a pointer into the data_ if it exists. Otherwise NULL.
	VS_FIXEDFILEINFO* fixed_file_info_;	
};
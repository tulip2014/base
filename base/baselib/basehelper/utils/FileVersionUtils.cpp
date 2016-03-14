
#include "FileVersionUtils.h"


#pragma comment(lib, "Version.lib")

namespace {

	typedef struct {
		WORD language;
		WORD code_page;
	} LanguageAndCodePage;

}


CFileVersionUtils::CFileVersionUtils( void* data, int language, int code_page )
	: language_(language)
	, code_page_(code_page)
	, data_((char*)data)
{
	fixed_file_info_ = NULL;
	UINT size;
	::VerQueryValue(data_, L"\\", (LPVOID*)&fixed_file_info_, &size);
}

CFileVersionUtils::~CFileVersionUtils()
{
	if (data_)
	{
		free(data_);
		data_ = NULL;
	}
}

CFileVersionUtils* CFileVersionUtils::CreateCFileVersionUtils( const std::wstring& file_path )
{
	DWORD dummy;	
	DWORD length = ::GetFileVersionInfoSizeW(file_path.c_str(), &dummy);
	if (length == 0)
		return NULL;

	void* data = calloc(length, 1);
	if (!data)
		return NULL;

	if (!::GetFileVersionInfoW(file_path.c_str(), dummy, length, data)) {
		free(data);
		return NULL;
	}

	LanguageAndCodePage* translate = NULL;
	UINT page_count;
	BOOL query_result = ::VerQueryValueW(data, L"\\VarFileInfo\\Translation", (void**)&translate, &page_count);

	if (query_result && translate) {
		return new CFileVersionUtils(data, translate->language, translate->code_page);

	} else {
		free(data);
		return NULL;
	}
}

std::wstring CFileVersionUtils::company_name() {
	return GetStringValue(L"CompanyName");
}

std::wstring CFileVersionUtils::company_short_name() {
	return GetStringValue(L"CompanyShortName");
}

std::wstring CFileVersionUtils::internal_name() {
	return GetStringValue(L"InternalName");
}

std::wstring CFileVersionUtils::product_name() {
	return GetStringValue(L"ProductName");
}

std::wstring CFileVersionUtils::product_short_name() {
	return GetStringValue(L"ProductShortName");
}

std::wstring CFileVersionUtils::comments() {
	return GetStringValue(L"Comments");
}

std::wstring CFileVersionUtils::legal_copyright() {
	return GetStringValue(L"LegalCopyright");
}

std::wstring CFileVersionUtils::product_version() {
	return GetStringValue(L"ProductVersion");
}

std::wstring CFileVersionUtils::file_description() {
	return GetStringValue(L"FileDescription");
}

std::wstring CFileVersionUtils::legal_trademarks() {
	return GetStringValue(L"LegalTrademarks");
}

std::wstring CFileVersionUtils::private_build() {
	return GetStringValue(L"PrivateBuild");
}

std::wstring CFileVersionUtils::file_version() {
	return GetStringValue(L"FileVersion");
}

std::wstring CFileVersionUtils::original_filename() {
	return GetStringValue(L"OriginalFilename");
}

std::wstring CFileVersionUtils::special_build() {
	return GetStringValue(L"SpecialBuild");
}

std::wstring CFileVersionUtils::last_change() {
	return GetStringValue(L"LastChange");
}

bool CFileVersionUtils::is_official_build() {
	return (GetStringValue(L"Official Build").compare(L"1") == 0);
}

bool CFileVersionUtils::GetValue(const wchar_t* name,
							   std::wstring* value_str) 
{
	WORD lang_codepage[8];
	int i = 0;
	// Use the language and codepage from the DLL.
	lang_codepage[i++] = language_;
	lang_codepage[i++] = code_page_;
	// Use the default language and codepage from the DLL.
	lang_codepage[i++] = ::GetUserDefaultLangID();
	lang_codepage[i++] = code_page_;
	// Use the language from the DLL and Latin codepage (most common).
	lang_codepage[i++] = language_;
	lang_codepage[i++] = 1252;
	// Use the default language and Latin codepage (most common).
	lang_codepage[i++] = ::GetUserDefaultLangID();
	lang_codepage[i++] = 1252;

	i = 0;
	while (i < 8) {
		wchar_t sub_block[MAX_PATH];
		WORD language = lang_codepage[i++];
		WORD code_page = lang_codepage[i++];
		_snwprintf_s(sub_block, MAX_PATH, MAX_PATH,
			L"\\StringFileInfo\\%04x%04x\\%ls", language, code_page, name);
		LPVOID value = NULL;
		UINT size;
		BOOL r = ::VerQueryValueW(data_, sub_block, &value, &size);
		if (r && value) {
			value_str->assign(static_cast<wchar_t*>(value));
			return true;
		}
	}
	return false;
}

std::wstring CFileVersionUtils::GetStringValue(const wchar_t* name) 
{
	std::wstring str;
	if (GetValue(name, &str))
		return str;
	else
		return L"";
}

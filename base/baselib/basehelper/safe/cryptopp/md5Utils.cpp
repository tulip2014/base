

#include "md5Utils.h"
#include "../../utils/StringUtils.h"
#include <strsafe.h>

#ifdef _DEBUG
#pragma comment(lib, "cryptlibd.lib")
#else
#pragma comment(lib, "cryptlib.lib")
#endif

using namespace CryptoPP;

namespace safe { namespace crypt {
	std::wstring GetMD5W(LPCWSTR lpStr)
	{
		if (lpStr == NULL || _tcslen(lpStr) == 0)
		{
			return L"";
		}

		return StringUtils::s2ws(GetMD5A(StringUtils::ws2s(lpStr).c_str()));
	}

	std::string GetMD5A(LPCSTR lpStr)
	{
		char buf[33] = {0};
		byte mres[16];

		Weak::MD5 md5; 
		md5.Update((byte*)lpStr, strlen(lpStr));
		md5.Final(mres);       

		char* temp = buf;
		for(int i = 0; i<16; i++)
		{
			sprintf_s(temp, 3, "%02X", mres[i]);
			temp += 2;
		}

		return std::string(buf);
	}
}}
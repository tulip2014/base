
#include "base64Utils.h"

#ifdef _DEBUG
#pragma comment(lib, "cryptlibd.lib")
#else
#pragma comment(lib, "cryptlib.lib")
#endif

using namespace CryptoPP;

namespace safe { namespace crypt {
	std::wstring Base64EncodeW(LPCWSTR lpStr)
	{
		char cStr[5] = "asdf";
		Base64Encoder encoder;
		encoder.PutMessageEnd((byte*)cStr, 5, -1, false);
		//const byte ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
		//AlgorithmParameters params = MakeParameters(Name::EncodingLookupArray(),(const byte *)ALPHABET);
		//encoder.IsolatedInitialize(params);
		CryptoPP::lword a = encoder.MaxRetrievable();
		std::string sTest;
		sTest.resize(a);
		encoder.Get((byte*)&sTest[0], a);

		return L"";
	}
}}
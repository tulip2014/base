
/********************************************************************
	created:	2016/03/14
	created:	14:3:2016   10:58
	filename: 	base\baselib\basehelper\safe\md5.h
	file path:	base\baselib\basehelper\safe
	file base:	md5
	file ext:	h
	author:		murisly
	
	purpose:	to calc md5
*********************************************************************/

/***********************************************************************
char sResult[33] = {0};
MD5 test("test");
test.toString(sResult);

MD5 md5;
md5.update(std::ifstream(ptszFilePath,std::ios::binary));
std::wstring strMD5 = StringsUtils::s2ws(md5.toString());
************************************************************************/

#pragma once

#include <string>
#include <fstream>


/* Type define */
typedef unsigned char byte;
typedef unsigned int uint32;

using std::string;
using std::ifstream;

namespace safe {
	/* MD5 declaration. */
	class MD5 {
	public:
		MD5();
		MD5(const void* input, size_t length);
		MD5(const string& str);
		MD5(ifstream& in);
		void update(const void* input, size_t length);
		void update(const string& str);
		void update(ifstream& in);
		const byte* digest();
		string toString();
		void toString(char* buf,int nLength=33);
		void reset();
		static string bytesToHexString(const byte* input, size_t length);

	private:
		void update(const byte* input, size_t length);
		void final();
		void transform(const byte block[64]);
		void encode(const uint32* input, byte* output, size_t length);
		void decode(const byte* input, uint32* output, size_t length);

		/* class uncopyable */
		MD5(const MD5&);
		MD5& operator=(const MD5&);

	private:
		uint32 _state[4];	/* state (ABCD) */
		uint32 _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
		byte _buffer[64];	/* input buffer */
		byte _digest[16];	/* message digest */
		bool _finished;		/* calculate finished ? */

		static const byte PADDING[64];	/* padding for calculate */
		static const char HEX[16];
		enum { BUFFER_SIZE = 1024 };
	};
}




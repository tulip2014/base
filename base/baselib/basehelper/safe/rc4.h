/********************************************************************
	created:	2016/03/16
	created:	16:3:2016   23:30
	filename: 	E:\work\git\base\base\base\baselib\basehelper\safe\rc4.h
	file path:	E:\work\git\base\base\base\baselib\basehelper\safe
	file base:	rc4
	file ext:	h
	author:		
	version:    1.0.0
	purpose:	
*********************************************************************/

namespace safe{

	int RC4_Encrypt(unsigned char *pRC4Key, int nKeyLen, unsigned char *pCipher, int nMaxSize, const unsigned char *pMessage, int nMessageLength);
	int RC4_Decrypt(unsigned char *pRC4Key, int nKeyLen, unsigned char *pMessage, int nMaxSize, const unsigned char *pCipher, int nCipherLength);


#define BOX_LEN 256
#define ARC4_KEY "!@#$%^&*()_+|UDPARC4KEYFORHARDKORE1.0.0PACKET1234567890*Talkwithfriends,family,orteammatesoverafastandclearconnection*Abilitytousetextchatforanyonewithoutamicrophone*Nativeclientandserversupportforall3majorplatforms*AutomaticmicrophonevolumenormalizationAd"

	char* Encrypt(const char* szSource, const char* szPassWord); // 加密，返回加密结果
	char* Decrypt(const char* szSource, const char* szPassWord); // 解密，返回解密结果

}
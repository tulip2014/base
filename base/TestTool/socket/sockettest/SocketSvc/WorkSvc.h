
#ifndef    WORKSVC_H
#define    WORKSVC_H

#include <tchar.h>
#include <winsock2.h>
#include <windows.h>


class ServiceObj
{
public:
	ServiceObj();
	~ServiceObj();
	BOOL Init(LPCSTR lpIPaddr, unsigned short uPort);
	BOOL Start();

private:
	sockaddr_in m_soAddr;
	WSADATA m_wsadata;
	SOCKET m_sConnect;
};




#endif
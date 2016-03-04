
#ifndef  WORKCLIENT_H
#define  WORKCLIENT_H

#include <tchar.h>
#include <winsock2.h>
#include <windows.h>


class ClientObj
{
public:
	ClientObj();
	~ClientObj();
	BOOL Init();
	BOOL Start();
	BOOL CleanUp();

private:
	WSADATA m_wsadata;
	SOCKET  m_socket;
	sockaddr_in m_addr;
};



#endif
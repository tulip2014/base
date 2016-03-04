
#include "stdafx.h"
#include "WorkSvc.h"
#include <iostream>
using namespace std;

#pragma comment(lib, "WS2_32.lib")

ServiceObj::ServiceObj()
{}

ServiceObj::~ServiceObj()
{
}

BOOL ServiceObj::Init(LPCSTR lpIPaddr, unsigned short uPort)
{
	m_soAddr.sin_family = AF_INET;
	m_soAddr.sin_addr.S_un.S_addr = inet_addr(lpIPaddr);
	m_soAddr.sin_port = uPort;

	return true;
}

BOOL ServiceObj::Start()
{
	WORD dw = MAKEWORD(2, 0);
	WSAStartup(dw, &m_wsadata);
	m_sConnect = ::socket(AF_INET, SOCK_STREAM, 0);
	::bind(m_sConnect, (sockaddr*)(&m_soAddr), sizeof(m_soAddr));
	::listen(m_sConnect, 5);
	cout<<"service start"<<endl;

	SOCKET soRet;
	sockaddr_in sAddr;
	int n = sizeof(sAddr);
	while (true)
	{
		soRet = ::accept(m_sConnect, (sockaddr*)&sAddr, &n);
		if (NULL != soRet)
		{
			cout<<"already connert"<<inet_ntoa(sAddr.sin_addr)<<endl;
			::send(soRet, "welcome!", strlen("welcome!"), 0);
		}
	}


	
}
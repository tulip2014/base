
#include "stdafx.h"
#include "workclient.h"

#pragma comment(lib, "WS2_32.lib")

ClientObj::ClientObj()
{}

ClientObj::~ClientObj()
{}

BOOL ClientObj::Init()
{
	return true;
}

BOOL ClientObj::Start()
{
	WORD w = MAKEWORD(2, 0);
	WSAStartup(w, &m_wsadata);
	m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	m_addr.sin_family = AF_INET;
	m_addr.sin_port = 75;
	m_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	char sRecv[MAX_PATH] = {0};
	::connect(m_socket, (sockaddr*)&m_addr, sizeof(m_addr));
	::recv(m_socket, sRecv, MAX_PATH, 0);

	return 0;
}

BOOL ClientObj::CleanUp()
{
	if (NULL != m_socket)
	{
		::closesocket(m_socket);
	}

	WSACleanup();
	return true;
}
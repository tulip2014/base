// SockClient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "workclient.h"

int _tmain(int argc, _TCHAR* argv[])
{
	ClientObj test;
	test.Start();

	test.CleanUp();

	return 0;
}


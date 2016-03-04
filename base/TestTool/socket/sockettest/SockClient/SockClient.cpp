// SockClient.cpp : 定义控制台应用程序的入口点。
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


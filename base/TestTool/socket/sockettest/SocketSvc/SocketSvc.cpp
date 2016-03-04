// SocketSvc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "WorkSvc.h"





int _tmain(int argc, _TCHAR* argv[])
{
	ServiceObj test;
	test.Init("127.0.0.1", 75);
	test.Start();

	return 0;
}


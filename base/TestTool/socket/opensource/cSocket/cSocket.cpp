// cSocket.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Socket.h"

#include <iostream>

#pragma comment (lib, "Ws2_32.lib")
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{

	try {
		SocketClient s("www.baidu.com", 80);

		s.SendLine("GET / HTTP/1.0");
		s.SendLine("Host: www.baidu.com");
		s.SendLine("");

		while (1) {
			string l = s.ReceiveLine();
			if (l.empty()) break;
			cout << l;
			cout.flush();
		}

	} 
	catch (const char* s) {
		cerr << s << endl;
	} 
	catch (std::string s) {
		cerr << s << endl;
	} 
	catch (...) {
		cerr << "unhandled exception\n";
	}

	return 0;
}


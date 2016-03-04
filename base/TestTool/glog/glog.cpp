// glog.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <glog/logging.h>  

int _tmain(int argc, _TCHAR* argv[])
{

	FLAGS_log_dir = "D:\\glog";
	FLAGS_max_log_size = 4;  //最大日志文件大小 4M 
	google::InitGoogleLogging("abc.exe");
	LOG(INFO) << "test";
	google::ShutdownGoogleLogging(); 

	return 0;
}


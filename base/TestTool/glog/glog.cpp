// glog.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <glog/logging.h>  

int _tmain(int argc, _TCHAR* argv[])
{

	FLAGS_log_dir = "D:\\glog";
	FLAGS_max_log_size = 4;  //�����־�ļ���С 4M 
	google::InitGoogleLogging("abc.exe");
	LOG(INFO) << "test";
	google::ShutdownGoogleLogging(); 

	return 0;
}


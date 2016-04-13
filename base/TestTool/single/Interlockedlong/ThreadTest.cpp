#include "stdafx.h"
#include "ThreadTest.h"

ThreadTest::ThreadTest(int num){
	dwData = NULL;
	iThreadID = num;
}

ThreadTest::~ThreadTest(){}

void ThreadTest::SetData(DWORD* data){
	dwData = data;
}

UINT ThreadTest::_ThreadProc()
{
	while (*dwData < 1000)
	{
		InterlockedExchange(dwData, ++(*dwData));

		TCHAR szNum[MAX_PATH] = {0};
		_stprintf_s(szNum, MAX_PATH, _T("thread:%d, num:%d\n\r"), iThreadID, *dwData);
		OutputDebugString(szNum);
	}

	return 0;
}
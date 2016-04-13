
#pragma once
#include "..\..\..\baselib\basehelper\process\ThreadUtils.h"
#include <Windows.h>

class ThreadTest : public CThreadUtils<ThreadTest>
{
public:
	ThreadTest(int);
	~ThreadTest(void);

	virtual UINT _ThreadProc();

	void SetData(DWORD* data);

private:
	DWORD* dwData;
	int iThreadID;
};
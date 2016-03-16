/********************************************************************
	created:	2016/03/16
	created:	16:3:2016   14:49
	filename: 	base\baselib\basehelper\process\ThreadUtilsTest.h
	file path:	base\baselib\basehelper\process
	file base:	ThreadUtilsTest
	file ext:	h
	author:		murisly
	
	purpose:	threadutils test
*********************************************************************/

#pragma once


#include "..\\process\\ThreadUtils.h"
#include "windows.h"


class ThreadUtilsTest : public CThreadUtils<ThreadUtilsTest> 
{
public:
	ThreadUtilsTest(void);
	~ThreadUtilsTest(void);

	virtual UINT _ThreadProc();

private:
};

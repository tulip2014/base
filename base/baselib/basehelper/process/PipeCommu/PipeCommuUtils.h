
/********************************************************************
	created:	2016/04/18
	created:	18:4:2016   14:45
	filename: 	baselib\basehelper\process\PipeCommu\PipeCommuUtils.h
	file path:	baselib\basehelper\process\PipeCommu
	file base:	PipeCommuUtils
	file ext:	h
	author:		Author
	
	purpose:	to built pipe communication server and client
*********************************************************************/

#pragma once

#include "PipeUtils.h"


class PipeCommuUtils
{
private:
	PipeUtils m_PipeUtils;
public:
	PipeCommuUtils();
	~PipeCommuUtils();
	int StartPipeServer(LPCWSTR lpPipeName);
};
/********************************************************************
	created:	2016/03/10
	created:	10:3:2016   17:09
	filename: 	base\baselib\basehelper\common\Singleton.h
	file path:	base\baselib\basehelper\common
	file base:	Singleton
	file ext:	h
	author:		murisly
	
	purpose:	singleton pattern
*********************************************************************/

#pragma once


template<class T>
class SingletonStatic
{
private:
	SingletonStatic(){}

public:
	static T* getInstance()
	{
		static T inst;
		return &inst;
	}
};

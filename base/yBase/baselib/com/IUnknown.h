
#pragma once
#include <iostream>
#include <objbase.h>

class IUnknow
{
public:
	virtual void PrintI() = 0;
	virtual ULONG AddRef() = 0;
	virtual ULONG Release() = 0;
	virtual HRESULT QueryInterface(const int& iID, void** ppv) = 0;
};

class IX : public IUnknow
{
public:
	virtual void PrintX() = 0;
};

class IY : public IUnknow
{
public:
	virtual void PrintY() = 0;
};

IUnknow* CreateInstance();
  
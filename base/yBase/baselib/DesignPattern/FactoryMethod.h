
#pragma once
#include <iostream>
using std::cout;
using std::endl;

class COsProduct
{
public:
	COsProduct(){};
	virtual ~COsProduct(){};
	virtual void Start() = 0;
};

class CWindowsProduct : public COsProduct
{
public:
	CWindowsProduct(){};
	virtual ~CWindowsProduct(){};
	virtual void Start()
	{
		//cout<<"windows start"<<endl;
	}
};

class CLinuxProduct : public COsProduct
{
public:
	CLinuxProduct(){};
	virtual ~CLinuxProduct(){};
	virtual void Start()
	{
		//cout<<"linux start"<<endl;
	}
};


//factory create base class
class COsCreateor
{
public:
	COsCreateor(){};
	virtual ~COsCreateor(){};
	virtual COsProduct* Create() = 0; 
};

class CWindowsCreate : public COsCreateor
{
public:
	CWindowsCreate(){};
	virtual ~CWindowsCreate(){};
	virtual COsProduct* Create()
	{
		//cout<<"create windows"<<endl;
		return new CWindowsProduct;
	}
};

template <typename T>
class CConcreteCreator : public COsCreateor
{
public:
	CConcreteCreator(std::wstring strClassName) : m_strClassName(strClassName) {}
	virtual COsProduct* Create()
	{
		//cout<<"create ";
		//std::wcout<<m_strClassName.c_str()<<endl;
		return new T;
	}

private:
	std::wstring m_strClassName;
};
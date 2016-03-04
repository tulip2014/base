
#include "IUnknown.h"


class CUser : public IX,
	public IY
{
public:
	virtual ULONG AddRef();
	virtual ULONG Release();
	virtual void PrintI(){std::cout<<"i"<<std::endl;}
	virtual void PrintX(){std::cout<<"x"<<std::endl;}
	virtual void PrintY(){std::cout<<"y"<<std::endl;}
	virtual HRESULT QueryInterface(const int& iID, void** ppv);

	CUser() : m_cRef(0){};
	~CUser() {};
private:
	long m_cRef;
};

HRESULT CUser::QueryInterface(const int& iID, void** ppv)
{
	if (1 == iID)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if (2 == iID)
	{
		*ppv = static_cast<IX*>(this);
	}
	else if (3 == iID)
	{
		*ppv = static_cast<IY*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	((IUnknow*)*ppv)->AddRef();
	return S_OK;
}

ULONG CUser::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG CUser::Release()
{
	if (0 == InterlockedDecrement(&m_cRef))
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

IUnknow* CreateInstance()
{
	IUnknow* pI = static_cast<IX*>(new CUser);
	pI->AddRef();
	return pI;
}

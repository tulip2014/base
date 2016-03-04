// dllmain.h : 模块类的声明。

class CATLTestModule : public ATL::CAtlDllModuleT< CATLTestModule >
{
public :
	DECLARE_LIBID(LIBID_ATLTestLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLTEST, "{A0ED7E99-130A-409D-8C8E-F7477478558F}")
};

extern class CATLTestModule _AtlModule;

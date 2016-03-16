#include "PrivilegeUtils.h"

namespace privilege{


#define ACCESS_READ  1
#define ACCESS_WRITE 2
BOOL IsUserAnAdmin()
{
	HANDLE hToken;
	DWORD  dwStatus;
	DWORD  dwAccessMask;
	DWORD  dwAccessDesired;
	DWORD  dwACLSize;
	DWORD  dwStructureSize = sizeof(PRIVILEGE_SET);
	PACL   pACL            = NULL;
	PSID   psidAdmin       = NULL;
	BOOL   bReturn         = FALSE;
	PRIVILEGE_SET   ps;
	GENERIC_MAPPING GenericMapping;
	PSECURITY_DESCRIPTOR     psdAdmin           = NULL;
	SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;

	if(!ImpersonateSelf(SecurityImpersonation))
		goto LeaveIsAdmin;

	if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, FALSE, &hToken))
	{
		if (GetLastError() != ERROR_NO_TOKEN)
			goto LeaveIsAdmin;

		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
			goto LeaveIsAdmin;

		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
			goto LeaveIsAdmin;
	}

	if (!AllocateAndInitializeSid(&SystemSidAuthority, 2,
		SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0, &psidAdmin))
		goto LeaveIsAdmin;

	psdAdmin = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	if (psdAdmin == NULL)
		goto LeaveIsAdmin;

	if (!InitializeSecurityDescriptor(psdAdmin,
		SECURITY_DESCRIPTOR_REVISION))
		goto LeaveIsAdmin;

	dwACLSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) +
		GetLengthSid(psidAdmin) - sizeof(DWORD);

	pACL = (PACL)LocalAlloc(LPTR, dwACLSize);
	if (pACL == NULL)
		goto LeaveIsAdmin;

	if (!InitializeAcl(pACL, dwACLSize, ACL_REVISION2))
		goto LeaveIsAdmin;

	dwAccessMask= ACCESS_READ | ACCESS_WRITE;

	if (!AddAccessAllowedAce(pACL, ACL_REVISION2, dwAccessMask, psidAdmin))
		goto LeaveIsAdmin;

	if (!SetSecurityDescriptorDacl(psdAdmin, TRUE, pACL, FALSE))
		goto LeaveIsAdmin;

	if(!SetSecurityDescriptorGroup(psdAdmin, psidAdmin, FALSE))
		goto LeaveIsAdmin;
	if(!SetSecurityDescriptorOwner(psdAdmin, psidAdmin, FALSE))
		goto LeaveIsAdmin;

	if (!IsValidSecurityDescriptor(psdAdmin))
		goto LeaveIsAdmin;

	dwAccessDesired = ACCESS_READ;

	GenericMapping.GenericRead    = ACCESS_READ;
	GenericMapping.GenericWrite   = ACCESS_WRITE;
	GenericMapping.GenericExecute = 0;
	GenericMapping.GenericAll     = ACCESS_READ | ACCESS_WRITE;

	if (!AccessCheck(psdAdmin, hToken, dwAccessDesired,
		&GenericMapping, &ps, &dwStructureSize, &dwStatus, &bReturn))
		goto LeaveIsAdmin;

	if(!RevertToSelf())
		bReturn = FALSE;

LeaveIsAdmin:

	if (pACL) LocalFree(pACL);
	if (psdAdmin) LocalFree(psdAdmin);
	if (psidAdmin) FreeSid(psidAdmin);

	return bReturn;
}

BOOL SetPrivilegeEx(HANDLE hToken,LPCTSTR Privilege,BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	TOKEN_PRIVILEGES tpPrevious;
	DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES);

	if(!LookupPrivilegeValue(NULL,Privilege,&luid))
		return FALSE;

	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Luid       = luid;
	tp.Privileges[0].Attributes = 0;

	AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),&tpPrevious,&cbPrevious);
	DWORD dwError=GetLastError();
	if(dwError!=ERROR_SUCCESS && dwError!=ERROR_NOT_ALL_ASSIGNED)
		return FALSE;

	tpPrevious.PrivilegeCount		= 1;
	tpPrevious.Privileges[0].Luid	= luid;

	if(bEnablePrivilege)
		tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
	else
		tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED & tpPrevious.Privileges[0].Attributes);

	AdjustTokenPrivileges(hToken,FALSE,&tpPrevious,cbPrevious,NULL,NULL);

	dwError=GetLastError();
	if(dwError!=ERROR_SUCCESS && dwError!=ERROR_NOT_ALL_ASSIGNED)
		return FALSE;

	return TRUE;
}

BOOL WINAPI EnableProcessDebugPrivilege()
{
	BOOL	bOK=FALSE;
	HANDLE	hToken=NULL; 

	if(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES
		| TOKEN_ADJUST_SESSIONID | TOKEN_ADJUST_DEFAULT | TOKEN_ASSIGN_PRIMARY | TOKEN_DUPLICATE,&hToken)
		&& SetPrivilegeEx(hToken,SE_DEBUG_NAME,TRUE) && SetPrivilegeEx(hToken,SE_TCB_NAME,TRUE))
		bOK=TRUE;

	//	ShowTokenInformation(hToken);

	if(hToken!=NULL)
		CloseHandle(hToken);

	return bOK;
}

}
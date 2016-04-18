
#include "..\..\stdafx.h"
#include "PipeUtils.h"

PipeUtils::PipeUtils()
{
	dwSendBufferLength = MAX_PATH;
	dwReceiveBufferLength = MAX_PATH;
	_tcscpy_s(szPipeName, MAX_PATH, PIPENAME);
}

PipeUtils::~PipeUtils()
{
	close();
}

int PipeUtils::close()
{
	return 0;
}

int CreateSecurityAttributes(PSECURITY_ATTRIBUTES pSecurityAttribute, BOOL bNeedDescriptor = TRUE, BOOL bInheritHandle = FALSE)
{
	if ( !pSecurityAttribute )
		return 0;

	int v3 = 0;
	PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL;
	pSecurityAttribute->lpSecurityDescriptor = pSecurityDescriptor;
	pSecurityAttribute->bInheritHandle = bInheritHandle;
	pSecurityAttribute->nLength = sizeof(SECURITY_ATTRIBUTES);

	if ( !bNeedDescriptor )
	{
		return 1;
	}

	pSecurityDescriptor = LocalAlloc(LMEM_ZEROINIT, sizeof(SECURITY_DESCRIPTOR));
	if ( pSecurityDescriptor
		&& InitializeSecurityDescriptor(pSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION)
		&& SetSecurityDescriptorDacl(pSecurityDescriptor, TRUE, NULL, FALSE) )
	{
		pSecurityAttribute->lpSecurityDescriptor = pSecurityDescriptor;
		return 2;
	}
	return 0;
}

int FreeSecurityAttributes(PSECURITY_ATTRIBUTES lpSecurityAttribute)
{
	if ( lpSecurityAttribute && lpSecurityAttribute->lpSecurityDescriptor )
	{
		LocalFree(lpSecurityAttribute->lpSecurityDescriptor);
		lpSecurityAttribute->lpSecurityDescriptor = 0;
	}
	return 1;
}

HANDLE PipeUtils::CreateNewPipe(LPCWSTR lpName)
{
	HANDLE hResult = NULL;
	HANDLE hPipe = NULL;
	SECURITY_ATTRIBUTES SecurityAttributes = {0};

	if ( CreateSecurityAttributes(&SecurityAttributes) )
	{
		hPipe = CreateNamedPipe(
			lpName,
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,  
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
			PIPE_UNLIMITED_INSTANCES,
			m_dwSendBufferLength,
			m_dwReceiveBufferLength,
			0,
			&SecurityAttributes);

		FreeSecurityAttributes(&SecurityAttributes);
		hResult = hPipe;
	}

	return hResult;
}
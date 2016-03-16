/********************************************************************
	created:	2016/03/04
	created:	4:3:2016   16:26
	filename: 	OSHelper.h
	file path:	
	file base:	OSHelper
	file ext:	h
	author:		murisly
	
	purpose:	
*********************************************************************/

#pragma once

#include "Winbase.h"

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);

namespace OSUtils {
	BOOL IsX86();
	BOOL IsWow64();

	BOOL RebootSystem();

	BOOL IsWin7OrLater();
	BOOL IsVistaOrLater();
	BOOL IsWinXPOrLater();
	BOOL IsWin2k3();
};
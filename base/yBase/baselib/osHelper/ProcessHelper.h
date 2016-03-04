
#ifndef		_PROCESSHELPER_H_
#define		_PROCESSHELPER_H_

#include <string>
#include <vector>
#include <windows.h>

namespace base
{
	namespace proc
	{
		DWORD EnumProcess(std::vector<std::wstring>& vecProcess);
	}
}

#endif // _PROCESSHELPER_H_

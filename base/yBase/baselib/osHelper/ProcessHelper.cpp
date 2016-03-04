
#include "ProcessHelper.h"
#include "TlHelp32.h"

namespace base { namespace proc {

	DWORD EnumProcess(std::vector<std::wstring>& vecProcess)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe = {0};
		pe.dwSize = sizeof(PROCESSENTRY32);

		if(Process32First(hSnapshot, &pe))
		{
			do
			{
				vecProcess.push_back(pe.szExeFile);
			} while(Process32Next(hSnapshot, &pe));
		}
		return 0;
	}

}}
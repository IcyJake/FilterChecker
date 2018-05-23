/*
	This exploit/program was made by IcyJake! Please give proper credits.
*/



#include "Includes.h"

namespace memory
{
	bool compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask)
			{
				return 0;
			}

		return (*szMask) == NULL;
	}

	DWORD findPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
	{
		for (int i = 0; i < (int)dwLen; i++)
		{
			if (compare((BYTE*)(dwAddress + (int)i), bMask, szMask))
			{
				return (int)(dwAddress + i);
			}
		}

		return 0;
	}

	int scan(DWORD mode, char* content, char* mask)
	{
		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;

		for (DWORD lpAddr = 0; lpAddr < 0x7FFFFFFF; lpAddr += PageSize)
		{
			DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);

			if (vq == ERROR_INVALID_PARAMETER || vq == 0)
			{
				break;
			}
			if (mi.Type == MEM_MAPPED)
			{
				continue;
			}
			if (mi.Protect == mode)
			{
				int address = findPattern(lpAddr, PageSize, (PBYTE)content, mask);
				if (address != 0)
				{
					return address;
				}
			}
		}

		return 0;
	}
}
#include "Includes.h"

DWORD base = (DWORD)GetModuleHandle(NULL);
#define aslr(x) (x - 0x00400000 + base)
int WS_VT = aslr(0x0113B13C);

BOOL check(int workspace)
{
	return *(BYTE*)(workspace + 0x416);
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, [](void*) -> DWORD
		{
			MessageBox(NULL, (check(memory::scan(PAGE_READWRITE, (char*)&WS_VT, (char*)"xxxx")) ? TEXT("True") : TEXT("False")), TEXT("FilteringEnabled"), MB_OK);
			return 0;
		}, NULL, NULL, NULL);
			break;
	}
}

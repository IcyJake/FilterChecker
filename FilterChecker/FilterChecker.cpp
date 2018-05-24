/*
	This exploit/program was made by IcyJake! Please give proper credits.
*/



#include "Includes.h"

DWORD base = (DWORD)GetModuleHandle(L"RobloxPlayerBeta.exe");
DWORD aslr(int address)
{
	return (address - 0x400000 + base);
}

int dataModelAddress = aslr(0x1103808); // Address last updated: 5/23/18

void consoleBypass()
{
	DWORD nothing;

	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nothing);

	*(BYTE*)(&FreeConsole) = 0xC3;
}

namespace roblox
{
	int dataModel;
	int workspace;

	std::string* getName(int instance)
	{
		return (std::string*)(*(int*)(instance + 0x28));
	}

	const char* getClass(int self)
	{
		return (const char*)(*(int(**)(void))(*(int*)self + 16))() + 4;
	}

	int findFirstChild_PartOf(int instance, std::string name)
	{
		DWORD startOfChildren = *(DWORD*)(instance + 0x2C);
		DWORD endOfChildren = *(DWORD*)(startOfChildren + 4);

		for (int i = *(int*)startOfChildren; i != endOfChildren; i += 8)
		{
			if (strstr(getName(*(int*)i) -> c_str(), name.c_str()))
			{
				return *(int*)i;
			}
		}

		return 0;
	}

	bool filterCheck()
	{
		if (*(BYTE*)(workspace + 0x416) == 0)
		{
			return false;
		}
		
		return true;
	}
}

void console(char* name)
{
	AllocConsole();
	SetConsoleTitleA(name);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	HWND ConsoleHandle = GetConsoleWindow();

	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

void scan()
{
	roblox::dataModel = memory::scan(PAGE_READWRITE, (char*)&dataModelAddress, "xxxx");
	roblox::workspace = roblox::findFirstChild_PartOf(roblox::dataModel, "Workspace");
}

int main()
{
	std::string key = "check";

	consoleBypass();

	console("FilterChecker - Made by IcyJake/GhostX @ V3rmillion");

	std::cout << "Loading...\n\n";

	scan();

	std::cout << "Welcome to FilterChecker!\n\n\n\n";
	std::cout << "Please type 'check' to check a game's filter.\n\n";
	std::cin >> key;
	std::cout << "\n\n\n";

	if (key != "check")
	{
		std::cout << "[Error] -> Please restart and type 'check'.\n\n";
	}
	
	if (key == "check")
	{
		if (roblox::filterCheck())
		{
			std::cout << "[Success] -> Filtering is enabled! :(\n";
		}
		
		if (!roblox::filterCheck())
		{
			std::cout << "[Success] -> Filtering is disabled! :)\n";
		}
	}

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		HANDLE hThread = NULL;
		HANDLE hDllMainThread = OpenThread(THREAD_ALL_ACCESS, NULL, GetCurrentThreadId());

		if (lpReserved == NULL)
		{
			if (!(hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0)))
			{
				CloseHandle(hDllMainThread);

				return FALSE;
			}

			CloseHandle(hThread);
		}
		else if (dwReason == DLL_PROCESS_DETACH) {}

		return TRUE;
	}
}
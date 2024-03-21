#include "Main.h"
#include "detours.h"
#include <map>
#include <assert.h>
#include <tchar.h>
typedef BOOL(WINAPI* SetCursorPosFunc)(int x, int y);
SetCursorPosFunc SetCursorPosPtr = NULL;

typedef BOOL(WINAPI* GetCursorPosFunc)(LPPOINT lpPoint);
GetCursorPosFunc GetCursorPosPtr = NULL;

HMODULE g_hModule = NULL;
int mouse_x;
int mouse_y;
char* m_pszDllPath;

int OldMouseX = 0;
int OldMouseY = 0;
bool bMouse = false;

void UnHookLoop()
{
	while (1 == 1 && 0 == 0)
	{
		if (GetAsyncKeyState(VK_F12) & 1)
		{
			UnHook();
			Sleep(1000);
			FreeLibraryAndExitThread(g_hModule, 0);
		}
	}
}

void HandleMousePos(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}

bool APIENTRY pSetCursorPos(int x, int y)
{
	OldMouseX = x;
	OldMouseY = y;
	if (bMouse && !g_pEngineClient->Con_IsVisible()) return 1;
	return SetCursorPosPtr(x, y);
}

bool APIENTRY pGetCursorPos(LPPOINT lpPoint)
{
	bool ret = GetCursorPosPtr(lpPoint);
	if (bMouse && !g_pEngineClient->Con_IsVisible())
	{
		ScreenToClient(GetForegroundWindow(), lpPoint);
		HandleMousePos(lpPoint->x, lpPoint->y);
		lpPoint->x = OldMouseX;
		lpPoint->y = OldMouseY;
	}
	return ret;
}

__int64 pow(__int64 x, __int64 e, unsigned __int64 m)
{
	__int64 t;
	if (e == 0) return 1;
	if (e == 1) return x % m;
	else
	{
		t = pow(x, e / 2, m);
		if (e % 2 == 0) return t * t % m;
		else return t * t % m * x % m;
	}
}

void printconsole(const char* msg, ...)
{
	va_list va_alist;
	char szBuffer[2048];

	va_start(va_alist, msg);
	auto len = vsprintf_s(szBuffer, msg, va_alist);
	va_end(va_alist);

	szBuffer[len + 0] = '\r';
	szBuffer[len + 1] = '\n';
	szBuffer[len + 2] = '\0';
	len = len + 2;

	printf(szBuffer);
}

void runconsole()
{
	if (AllocConsole())
	{
		freopen( /*CONOUT$*/XorStr<0xEF, 8, 0x233C63F9>("\xAC\xBF\xBF\xBD\xA6\xA0\xD1" + 0x233C63F9).s, /*wt*/XorStr<0x84, 3, 0x99B21A1D>("\xF3\xF1" + 0x99B21A1D).s, stdout);
		SetConsoleTitleA("");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
}
#include "Security.h"
#include "Stuff.cpp"
#include "PaintTraverse.cpp"

void Set_Bytes(__int8 Modify_Access_Rights, void* Starting_Location, unsigned __int32 Bytes_Amount, unsigned __int8 Byte)
{
	unsigned long int Previous_Access_Rights;

	if (Modify_Access_Rights == 1)
	{
		VirtualProtect(Starting_Location, Bytes_Amount, PAGE_EXECUTE_READWRITE, &Previous_Access_Rights);
	}

	memset(Starting_Location, Byte, Bytes_Amount);

	if (Modify_Access_Rights == 1)
	{
		VirtualProtect(Starting_Location, Bytes_Amount, Previous_Access_Rights, &Previous_Access_Rights);
	}
}

int ezz(HANDLE hProcess)
{

	// Получаем приоритет текущего процесса
	int priority = GetPriorityClass(hProcess);

	// Если текущий приоритет ниже, чем желаемый, повышаем его
	if (priority < HIGH_PRIORITY_CLASS)
	{
		SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
	}

	// Устанавливаем аффинити текущего процесса, чтобы он использовал только одно ядро процессора
	HANDLE hCurrentThread = GetCurrentThread();
	DWORD_PTR affinityMask = 1; // Используем только первое ядро
	SetThreadAffinityMask(hCurrentThread, affinityMask);

	// Устанавливаем приоритет текущего потока
	SetThreadPriority(hCurrentThread, THREAD_PRIORITY_HIGHEST);

	// Возвращаем исходный приоритет процесса
	SetPriorityClass(hProcess, priority);

	return 0;
}

#include "XorStr.h"
#include "uid1488.h"
#include "mainshit.h"
char g_DllPath[MAX_PATH];
int __stdcall DllMain(HMODULE hMod, DWORD dwReason, PVOID lpReserved)
{
	TCHAR volumeName[MAX_PATH + 1] = { 0 };
	TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
	DWORD serialNumber = 0;
	DWORD maxComponentLen = 0;
	DWORD fileSystemFlags = 0;
	std::string name;

	int HWIDList[] =
	{	
		
		2022172064, // FeeD$1337
		1411083576, // codual
		-1676752611, // nightmare__
		-1768414456, // desync
		1524652599,	// moonrise	
		111493859,	// karkarich
		-1842360125, // slava ppcz 
	};
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if ((GetVolumeInformation)(
			(_T("C:\\")), 
			volumeName,
			ARRAYSIZE(volumeName),
			&serialNumber,
			&maxComponentLen,
			&fileSystemFlags,
			fileSystemName,
			ARRAYSIZE(fileSystemName)))
		{
			if (!GetVolumeInformation)
			{
				UnHook();
			}
			if (std::find(std::begin(HWIDList), std::end(HWIDList), serialNumber) != std::end(HWIDList)) {

				int max = sizeof(HWIDList) / sizeof(int);
			
				for (int i = 0; i < max; i++)
				{
					//uid = HWIDList[i];
					
					int hwid = HWIDList[i];
					uid = HWIDList[i];	
					//if hwid and name is correct, the cheat will inject, if not the game will crash

					if (hwid == serialNumber)
					{
						continue;
						
					}
				}
				Set_Bytes(1, (void*)((unsigned __int32)LoadLibraryW(L"vaudio_speex.dll") + 9360), 1, 195);
				DisableThreadLibraryCalls(hMod);
			

				SetCursorPosPtr = (SetCursorPosFunc)DetourFunction((LPBYTE)DetourFindFunction( /*user32.dll*/XorStr<0xA2, 11, 0x169109FE>("\xD7\xD0\xC1\xD7\x95\x95\x86\xCD\xC6\xC7" + 0x169109FE).s, /*SetCursorPos*/XorStr<0xA4, 13, 0xDA865A79>("\xF7\xC0\xD2\xE4\xDD\xDB\xD9\xC4\xDE\xFD\xC1\xDC" + 0xDA865A79).s), (LPBYTE)pSetCursorPos);
				GetCursorPosPtr = (GetCursorPosFunc)DetourFunction((LPBYTE)DetourFindFunction( /*user32.dll*/XorStr<0xB8, 11, 0x90902E99>("\xCD\xCA\xDF\xC9\x8F\x8F\x90\xDB\xAC\xAD" + 0x90902E99).s, /*GetCursorPos*/XorStr<0x10, 13, 0x624C4F36>("\x57\x74\x66\x50\x61\x67\x65\x78\x6A\x49\x75\x68" + 0x624C4F36).s), (LPBYTE)pGetCursorPos);
				g_hModule = hMod;
				GetModuleFileNameA(hMod, g_DllPath, sizeof(g_DllPath));
				m_pszDllPath = g_DllPath;
				g_Config.SetModule(hMod);

				HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Hook, hMod, 0, 0);
				ezz(thread);
				if (!thread)
				return 0;
				return 1;
			}

		}

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		UnHook();
	}

	return 1;
}

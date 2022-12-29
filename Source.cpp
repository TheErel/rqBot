#include <Windows.h>
#include "cheat.h"
#include <cstdint>
#include <string>
#include <fstream>
#include <cstring>
#pragma comment(lib, "user32")

WNDPROC g_oldWndProc;
HWND	hGame = FindWindow(0, "Royal Quest");
int		mainOffset = 0x7F7CA8;
int		CurrentWID = 0;
int		old_protect = 0;
float	gCordX = 0;
float	gCordY = 0;
DWORD	PlayerPosX = DeRef(DeRef((DWORD)GetModuleHandleA(0) + mainOffset) + 0x4) + 0x23C;
DWORD	PlayerPosY = DeRef(DeRef((DWORD)GetModuleHandleA(0) + mainOffset) + 0x4) + 0x240;
float	MCord = 10.0f;
float	MEvent = 5.0f;
DWORD	PlayerHealth = DeRef(DeRef((DWORD)GetModuleHandle(0) + mainOffset) + 0x4) + 0x204;
DWORD	MouseCord = DeRef(DeRef((DWORD)GetModuleHandle(0) + mainOffset) + 0x4) + 0x208;
DWORD	MouseEvent = DeRef(DeRef((DWORD)GetModuleHandle(0) + mainOffset) + 0x4) + 0x20C;
DWORD	accIdptr = ((DWORD)GetModuleHandle(0)) + 0x7FA124;
DWORD	WordlID = ((DWORD)GetModuleHandle(0)) + 0x6B0304;
DWORD	AdrSpeed = ((DWORD)GetModuleHandle(0)) + 0xB4B7C;
BOOL	BotStatus = 1, SpeedHack = 0, savePos = 0;																						// 40-49 991346588; 1387840084 204.1203766 124.2634048
BOOL	panicKey = 0;


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_KEYUP:
		{
			switch (wParam)
			{
				case VK_NUMPAD1:
				{
					BotStatus = !BotStatus;
					break;
				}
				case VK_NUMPAD2:
				{
					SpeedHack = !SpeedHack;
					break;
				}
				case VK_NUMPAD3:
				{
					savePos = !savePos;
					break;
				}
				case VK_NUMPAD6:
				{
					panicKey = !panicKey;
					break;
				}

			}
		}
	}
	return CallWindowProcA(g_oldWndProc, hwnd, message, wParam, lParam);
}

void accIdfunc()
{
	int idcur{ 0 };
	memRead(accIdptr, &idcur, 4);

	switch (idcur)
	{
		// Granas
	case 3156366: SetWindowText(hGame, "gstone3@mail.ru");
		break;
	case 3156368: SetWindowText(hGame, "gstone4@mail.ru");
		break;
	case 3156371: SetWindowText(hGame, "gstone6@mail.ru");
		break;
	case 3156372: SetWindowText(hGame, "gstone7@mail.ru");
		break;
	case 3156373: SetWindowText(hGame, "gstone8@mail.ru");
		break;
	case 3156374: SetWindowText(hGame, "gstone9@mail.ru");
		break;
	case 3156376: SetWindowText(hGame, "gstone10@mail.ru");
		break;
	case 3186928: SetWindowText(hGame, "gstone11@mail.ru");
		break;
	case 3186929: SetWindowText(hGame, "gstone12@mail.ru");
		break;
	case 3186930: SetWindowText(hGame, "gstone13@mail.ru");
		break;
	case 3186932: SetWindowText(hGame, "gstone15@mail.ru");
		break;
	default: SetWindowText(hGame, "Royal Quest");
		break;
	}
}

void memRead(DWORD address, void* buffer, DWORD size)
{
	DWORD oProtect{};
	VirtualProtect((void*)address, size, PAGE_EXECUTE_READWRITE, &oProtect);
	ReadProcessMemory(GetCurrentProcess(), (void*)address, buffer, size, 0);
	VirtualProtect((void*)address, size, oProtect, &oProtect);
}

void memWrite(DWORD address, void* buffer, DWORD size)
{
	DWORD oProtect{};
	VirtualProtect((void*)address, size, PAGE_EXECUTE_READWRITE, &oProtect);
	WriteProcessMemory(GetCurrentProcess(), (void*)address, buffer, size, 0);
	VirtualProtect((void*)address, size, oProtect, &oProtect);
}

UINT_PTR DeRef(UINT_PTR _uiptrPointer)
{
	UINT_PTR uiptrRet;
	DWORD oProtect = 0;
	VirtualProtect((LPVOID)_uiptrPointer, 4, PAGE_EXECUTE_READWRITE, &oProtect);
	if (!::ReadProcessMemory(GetCurrentProcess(), reinterpret_cast<LPVOID>(_uiptrPointer), &uiptrRet, sizeof(uiptrRet), NULL)) 
	{ 
		return 0UL;
	}
	VirtualProtect((LPVOID)_uiptrPointer, 4, oProtect, &oProtect);
	return uiptrRet;
}

void SendMouseClick(HWND hwnd, int x, int y)
{
	// Send the left mouse button down message to the window
	SendMessageA(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));

	// Send the left mouse button up message to the window
	SendMessageA(hwnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
}

DWORD WINAPI GameThread(LPVOID lParam)
{
	for (;; Sleep(200))

	{
		accIdfunc();
		std::fstream RQBotSettings;
		std::string pathRqBotSettings = ("C:\\Users\\greys\\source\\repos\\rqBot\\rqHelp.txt");
		RQBotSettings.open(pathRqBotSettings);
		RQBotSettings >> gCordX >> gCordY >> BotStatus;
		RQBotSettings.close();


		if (GetAsyncKeyState(VK_END) & 1)
		{
			SetWindowLong(hGame, GWLP_WNDPROC, (LONG)(UINT_PTR)g_oldWndProc);
			// Получаем хендл текущей DLL
			HMODULE hModule = GetModuleHandle(NULL);
			// Освобождаем ресурсы, занятые DLL
			CloseHandle(hModule);
			FreeLibrary(hModule);
			MessageBox(0, "Close Message", "Delete DLL", MB_OK);
		}
		if (savePos == 1)
		{

			float currentPosX{};
			float currentPosY{};
			memRead(PlayerPosX, &currentPosX, 4);
			memRead(PlayerPosY, &currentPosY, 4);
			std::ofstream file;
			file.open(pathRqBotSettings);
			file << currentPosX << "\n";
			file << currentPosY << "\n";
			file << 1;
			file.close();
			savePos = 0; //num3
		}
		if (panicKey == 1)
		{
			std::ofstream file2;
			file2.open(pathRqBotSettings);
			file2 << (float)1 << "\n";
			file2 << (float)2 << "\n";
			file2 << 0 << '\n';
			file2.close();
			panicKey = 0; //num6
		}
		if (SpeedHack == 1)
		{
			LONGLONG actSpeed{ 3246604332 };
			memWrite(AdrSpeed, &actSpeed, 4);
		}
		else if (SpeedHack == 0)
		{
			LONGLONG actSpeed{ 3246604316 };
			memWrite(AdrSpeed, &actSpeed, 4);
		}

		if (BotStatus == 1)
		{

			memRead(WordlID, &CurrentWID, 4);
			int CurrentHP{};
			memRead(PlayerHealth, &CurrentHP, 4);

			// Башня 
			if (CurrentWID == 1058262330)
			{
				Sleep(100);
				float CordX{ -7.430754662f };
				float CordY{ 8.960902214f };
				memWrite(PlayerPosX, &CordX, 4);
				memWrite(PlayerPosY, &CordY, 4);
				memWrite(MouseCord, &MCord, 4);
				memWrite(MouseEvent, &MEvent, 4);
				SendMouseClick(hGame, 343, 424);
				SendMouseClick(hGame, 335, 331);
			}
			// Мельница
			if (CurrentWID == 1046081534)
			{
				float CordX = -6.0f;
				float CordY = -6.0f;
				memWrite(PlayerPosX, &CordX, 4);
				memWrite(PlayerPosY, &CordY, 4);
				memWrite(MouseCord, &MCord, 4);
				memWrite(MouseEvent, &MEvent, 4);
				Sleep(100);
				SendMouseClick(hGame, 319, 232);
			}
			// Подвал с мельницы
			if (CurrentWID == (int)3194391926)
			{
				float CordX = -17.25160789f;
				float CordY = -0.2555446625f;
				memWrite(PlayerPosX, &CordX, 4);
				memWrite(PlayerPosY, &CordY, 4);
				memWrite(MouseCord, &MCord, 4);
				memWrite(MouseEvent, &MEvent, 4);
			}
			// ПвП 20+
			if (CurrentWID == 1012096358)
			{
				memWrite(PlayerPosX, &gCordX, 4);
				memWrite(PlayerPosY, &gCordY, 4);
				memWrite(MouseCord, &MCord, 4);
				memWrite(MouseEvent, &MEvent, 4);
			}
			if (CurrentHP == 0)
			{
				Sleep(4000);
				SendMouseClick(hGame, 414, 427);
				Sleep(1000);
				SendMouseClick(hGame, 493, 431);
				Sleep(200);
				SendMouseClick(hGame, 346, 213);

			}
		}

		else if (BotStatus == 0)
		{

		}
	}

}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwAttached, LPVOID lpvReserved)
{
	if (dwAttached == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL, 0, &GameThread, NULL, 0, NULL);
		g_oldWndProc = (WNDPROC)SetWindowLongPtrA(hGame, GWL_WNDPROC, (LONG_PTR)WndProc);
	}
	return 1;
}

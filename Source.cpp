#include <Windows.h>
#include "cheat.h"
#include <cstdint>
#include <string>
#include <fstream>
#include <cstring>
#pragma comment(lib, "user32")

WNDPROC g_oldWndProc;
HWND	hGame = FindWindow(0, "Royal Quest");
int		mainOffset = 0x7F7D08;
int		CurrentWID = 0;
int		old_protect = 0;
float	gCordX = 0;
float	gCordY = 0;
DWORD	PlayerPosX = DeRef(DeRef((DWORD)GetModuleHandleA(0) + mainOffset) + 0x4) + 0x23C;
DWORD	PlayerPosY = DeRef(DeRef((DWORD)GetModuleHandleA(0) + mainOffset) + 0x4) + 0x240;
DWORD   gameProtection = DeRef((DWORD)GetModuleHandleA(0) + 0x39EBAA);
float	MCord = 10.0f;
float	MEvent = 5.0f;
DWORD	PlayerHealth = DeRef(DeRef((DWORD)GetModuleHandle(0) + mainOffset) + 0x4) + 0x204;
DWORD	MouseCord = DeRef(DeRef((DWORD)GetModuleHandle(0) + mainOffset) + 0x4) + 0x208;
DWORD	MouseEvent = DeRef(DeRef((DWORD)GetModuleHandle(0) + mainOffset) + 0x4) + 0x20C;
DWORD	accIdptr = ((DWORD)GetModuleHandle(0)) + 0x7FA184;
DWORD	WordlID = ((DWORD)GetModuleHandle(0)) + 0x6B0304;
DWORD	AdrSpeed = ((DWORD)GetModuleHandle(0)) + 0xB4E1C;
BOOL	BotStatus = 1, SpeedHack = 0, savePos = 0;					// 40-49 991346588; 1387840084 204.1203766 124.2634048
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
		// ������ ID ��������� greysba@gmail.com
	case 3388832: SetWindowText(hGame, "1");
		break;
	case 3388833: SetWindowText(hGame, "2");
		break;
	case 3409453: SetWindowText(hGame, "3");
		break;
	case 3409454: SetWindowText(hGame, "4");
		break;
	case 3409455: SetWindowText(hGame, "5");
		break;
	case 3409456: SetWindowText(hGame, "6");
		break;
	case 3409457: SetWindowText(hGame, "7");
		break;
	case 3409458: SetWindowText(hGame, "8");
		break;
	case 3409459: SetWindowText(hGame, "9");
		break;
	case 3409460: SetWindowText(hGame, "10");
		break;
	case 3411802: SetWindowText(hGame, "11");
		break;
	case 3411804: SetWindowText(hGame, "12");
		break;
	case 3411805: SetWindowText(hGame, "13");
		break;
	case 3411806: SetWindowText(hGame, "14");
		break;
	case 3411807: SetWindowText(hGame, "15");
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
		int newGameProtection = 3505907059;
		memWrite(gameProtection, &newGameProtection, 4);

		accIdfunc();
		std::fstream RQBotSettings;
		std::string pathRqBotSettings = ("E:\\rqHelp.txt");
		RQBotSettings.open(pathRqBotSettings);
		RQBotSettings >> gCordX >> gCordY >> BotStatus;
		RQBotSettings.close();


		if (GetAsyncKeyState(VK_END) & 1)
		{
			SetWindowLong(hGame, GWLP_WNDPROC, (LONG)(UINT_PTR)g_oldWndProc);
			// �������� ����� ������� DLL
			HMODULE hModule = GetModuleHandle(NULL);
			// ����������� �������, ������� DLL
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

			// ����� 
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
			// ��������
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
			// ������ � ��������
			if (CurrentWID == (int)3194391926)
			{
				float CordX = -17.25160789f;
				float CordY = -0.2555446625f;
				memWrite(PlayerPosX, &CordX, 4);
				memWrite(PlayerPosY, &CordY, 4);
				memWrite(MouseCord, &MCord, 4);
				memWrite(MouseEvent, &MEvent, 4);
			}
			// ��� 20+
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

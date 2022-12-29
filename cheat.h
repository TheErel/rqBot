#pragma once
#include <Windows.h>
DWORD WINAPI GameThread(LPVOID lParam);
UINT_PTR DeRef(UINT_PTR _uiptrPointer);
LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void memRead(DWORD address, void* buffer, DWORD size);
void memWrite(DWORD address, void* buffer, DWORD size);
BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwAttached, LPVOID lpvReserved);
void accIdfunc();


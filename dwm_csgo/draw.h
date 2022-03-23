#pragma once
#include "Dwm_Dram.h"
#include "initData.h"
DWORD WINAPI dwmWork(LPVOID lpThreadParameter);


static LRESULT WINAPI wndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI dwmWorkEx(LPVOID lpThreadParameter);
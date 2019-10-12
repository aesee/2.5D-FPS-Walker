#pragma once
#include <Windows.h>
#include <chrono>

class Input
{
public:
	void UpdateInput(class Map& map);
	static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	std::chrono::time_point<std::chrono::system_clock> tp1;
	std::chrono::time_point<std::chrono::system_clock> tp2;
};
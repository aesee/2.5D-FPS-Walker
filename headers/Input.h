#pragma once
#include <Windows.h>
#include <chrono>

class Input
{
public:
	Input(HINSTANCE* inHInst);
	void UpdateInput(class GameConfig& gameConfig, class Player& player, class Map& map);
	static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE* hInst;
	std::chrono::time_point<std::chrono::system_clock> tp1;
	std::chrono::time_point<std::chrono::system_clock> tp2;

	struct InputKeys
	{
		bool W = false;
		bool S = false;
		bool A = false;
		bool D = false;
	};

	static InputKeys inputKeys;
};
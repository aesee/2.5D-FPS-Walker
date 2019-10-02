#pragma once
#include <Windows.h>
#include <memory>

class WindowApp
{
public:
	WindowApp();
	bool IsUserWantsToExit();
	int GetState() const;
	HINSTANCE* GetInstance();
	class Renderer* CreateRenderer(float FOV, float Depth);

private:
	LPCWSTR m_szWindowClass = L"SimpleFPSGameWindow";
	HINSTANCE m_hInstance;
	MSG m_msg;
	HACCEL m_hAccelTable;
	HWND m_hWnd;
};
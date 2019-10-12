#pragma once
#include <Windows.h>

class WindowApp
{
public:
	WindowApp();
	bool IsUserWantsToExit();
	int GetState() const;
	HINSTANCE* GetInstance();
	struct IntVector2D GetScreenSize();
	HDC GetDeviceContext();

private:
	LPCWSTR m_szWindowClass = L"SimpleFPSGameWindow";
	HINSTANCE m_hInstance;
	MSG m_msg;
	HACCEL m_hAccelTable;
	HWND m_hWnd;
};
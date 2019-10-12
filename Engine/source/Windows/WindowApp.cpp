#include "WindowApp.h"
#include <functional>
#include <utility>
#include "Resource.h"
#include "Input/InputCommand.h"
#include "EngineLib.h"
#include "Game.h"

WindowApp::WindowApp()
	: m_hInstance(GetModuleHandle(NULL)),
	m_hAccelTable(LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_SIMPLEFPS)))
{	
	// Register window class
	{
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WindowApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_hInstance;
		wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_SIMPLEFPS));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;		// Hide menu
		wcex.lpszClassName = m_szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);
	}	

	auto Test = GetLastError();

	m_hWnd = CreateWindowW(m_szWindowClass, L"Simple FPS Game", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInstance, nullptr);

	auto Test2 = GetLastError();

	if (!m_hWnd)
	{
		throw std::string("Can't create a window!");
	}

	STARTUPINFO StartupInfo;
	StartupInfo.dwFlags = 0;
	GetStartupInfo(&StartupInfo);

	ShowWindow(m_hWnd, (StartupInfo.dwFlags & STARTF_USESHOWWINDOW)
		? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);
}

bool WindowApp::IsUserWantsToExit()
{
	while (PeekMessage(&m_msg, 0, 0, 0, PM_REMOVE))
	{
		if (!TranslateAccelerator(m_msg.hwnd, m_hAccelTable, &m_msg))
		{
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}
	}

	return m_msg.message == WM_QUIT;
}

int WindowApp::GetState() const
{
	return (int)m_msg.wParam;
}

HINSTANCE* WindowApp::GetInstance()
{
	return &m_hInstance;
}

struct IntVector2D WindowApp::GetScreenSize()
{
	RECT rect;
	if (GetWindowRect(m_hWnd, &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		return IntVector2D(width, height);
	}

	throw std::string("Can't get a size of the screen!");
	return IntVector2D();
}

HDC WindowApp::GetDeviceContext()
{
	return GetDC(m_hWnd);
}

LRESULT WindowApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
	{
		Game& gameConfig = Game::Get();
		Controls& controls = gameConfig.GetControls();

		if (wParam == controls.turnLeft)
		{
			TurnLeftCommand Command;
			Command.Execute(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.turnRight)
		{
			TurnRightCommand Command;
			Command.Execute(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.moveForward)
		{
			MoveForwardCommand Command;
			Command.Execute(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.moveBackward)
		{
			MoveBackwardCommand Command;
			Command.Execute(gameConfig.GetCurrentPlayer());
		}

		return 0;
	}

	if (message == WM_KEYUP)
	{
		Game& gameConfig = Game::Get();
		Controls& controls = gameConfig.GetControls();

		if (wParam == controls.turnLeft)
		{
			TurnLeftCommand Command;
			Command.Undo(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.turnRight)
		{
			TurnRightCommand Command;
			Command.Undo(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.moveForward)
		{
			MoveForwardCommand Command;
			Command.Undo(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.moveBackward)
		{
			MoveBackwardCommand Command;
			Command.Undo(gameConfig.GetCurrentPlayer());
		}

		return 0;
	}

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}

	return 0;
}

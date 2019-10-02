#include "simpleFPS.h"
#include <functional>
#include <utility>
#include <Windows.h>
#include "GameConfig.h"
#include "Renderer.h"
#include "Map.h"
#include "Player.h"
#include "Input.h"

// Set main() as startup point for windows linker
#ifdef _MSC_VER
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

// Global Variables:
HINSTANCE hInst;                                // current instance

int main()
{
	STARTUPINFO StartupInfo;
	StartupInfo.dwFlags = 0;
	GetStartupInfo(&StartupInfo);

	HINSTANCE hInstance = GetModuleHandle(NULL);

	Renderer* renderer;
	GameConfig& gameConfig = GameConfig::Get();	
	Input input(&hInst);

	LPCWSTR szWindowClass = L"SimpleFPSGameWindow";
	// Register window class
	{
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Input::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPLEFPS));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;		// Hide menu
		wcex.lpszClassName = szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

		RegisterClassExW(&wcex);
	}

	// Perform application initialization:
	{
		hInst = hInstance; // Store instance handle in our global variable

		HWND hWnd = CreateWindowW(szWindowClass, L"Simple FPS Game", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

		if (!hWnd)
		{
			return FALSE;
		}

		ShowWindow(hWnd, (StartupInfo.dwFlags & STARTF_USESHOWWINDOW)
			? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		renderer = new Renderer(GetDC(hWnd), gameConfig.FOV, gameConfig.Depth);

		RECT rect;
		if (GetWindowRect(hWnd, &rect))
		{
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;

			renderer->SetScreenSize(width, height);
		}
	}

	// Init map
	Map map;
	renderer->SetMap(&map);

	// Create player
	Player player;

	// Game loop:
	MSG msg;
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMPLEFPS));
	while (true)
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

		// Input
		input.UpdateInput(gameConfig, player, map);

		// Render
		renderer->DrawFrame(player.GetPosition(), player.GetRotation());
	}

	delete renderer;

	return (int)msg.wParam;
}
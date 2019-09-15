#include "simpleFPS.h"
#include <utility>
#include <chrono>
#include <Windows.h>
#include "GameConfig.h"
#include "Renderer.h"
#include "Map.h"
#include "Player.h"

#define MAX_LOADSTRING 100

struct InputControls
{
	bool W = false;
	bool S = false;
	bool A = false;
	bool D = false;
};

InputControls input;
std::chrono::time_point<std::chrono::system_clock> tp1;
std::chrono::time_point<std::chrono::system_clock> tp2;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	DWORD dwBytesWritten = 0;
	
	// Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SIMPLEFPS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

	Renderer* renderer;
	GameConfig& gameConfig = GameConfig::Get();
    // Perform application initialization:
	{
		hInst = hInstance; // Store instance handle in our global variable

		HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

		if (!hWnd)
		{
			return FALSE;
		}

		ShowWindow(hWnd, nCmdShow);
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

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMPLEFPS));

    MSG msg;	

	Map map;
	renderer->SetMap(&map);

	Player player;

    // Game loop:    
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
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		if (input.A)
		{
			float newRotation = player.GetRotation() - (gameConfig.Speed * 0.75f) * fElapsedTime;
			player.SetRotation(newRotation);
		}

		if (input.D)
		{
			float newRotation = player.GetRotation() + (gameConfig.Speed * 0.75f) * fElapsedTime;
			player.SetRotation(newRotation);
		}

		if (input.W)
		{
			Vector2D playerPosition = player.GetPosition();
			float playerRotation = player.GetRotation();

			float movementSpeed = gameConfig.Speed * fElapsedTime;
			Vector2D offsetPosition = Vector2D(sinf(playerRotation) * movementSpeed, cosf(playerRotation) * movementSpeed);
			player.SetPosition(playerPosition + offsetPosition);

			if (map.IsWallIn((int)playerPosition.x, (int)playerPosition.y))
			{
				player.SetPosition(playerPosition);
			}
		}

		if (input.S)
		{
			Vector2D playerPosition = player.GetPosition();
			float playerRotation = player.GetRotation();

			float movementSpeed = gameConfig.Speed * fElapsedTime;
			Vector2D offsetPosition = Vector2D(sinf(playerRotation) * movementSpeed, cosf(playerRotation) * movementSpeed);
			player.SetPosition(playerPosition - offsetPosition);

			if (map.IsWallIn((int)playerPosition.x, (int)playerPosition.y))
			{
				player.SetPosition(playerPosition);
			}
		}

		// Render
		renderer->DrawFrame(player.GetPosition(), player.GetRotation());
	}

	delete renderer;

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPLEFPS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SIMPLEFPS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

// Catch input in window
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case 'A': {
			input.A = true;
			break;
		}
		case 'D': {
			input.D = true;
			break;
		}
		case 'W': {
			input.W = true;
			break;
		}
		case 'S': {
			input.S = true;
			break;
		}
		}

		return 0;
	}

	if (message == WM_KEYUP || message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case 'A': {
			input.A = false;
			break;
		}
		case 'D': {
			input.D = false;
			break;
		}
		case 'W': {
			input.W = false;
			break;
		}
		case 'S': {
			input.S = false;
			break;
		}
		}

		return 0;
	}	

	switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
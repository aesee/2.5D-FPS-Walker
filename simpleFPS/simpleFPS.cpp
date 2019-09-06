// simpleFPS.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "simpleFPS.h"

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>

#include <stdio.h>
#include <Windows.h>

#define MAX_LOADSTRING 100

struct GameConfig
{
	int ScreenWidth = 80;			// Console Screen Size X (columns)
	int ScreenHeight = 25;			// Console Screen Size Y (rows)
	int MapWidth = 16;				// World Dimensions
	int MapHeight = 16;

	float PlayerX = 14.7f;			// Player Start Position
	float PlayerY = 5.09f;
	float PlayerA = 0.0f;			// Player Start Rotation
	float FOV = 3.14159f / 3.0f;	// Field of View
	float Depth = 30.0f;			// Maximum rendering distance
	float Speed = 1.5f;			// Walking Speed
};

struct InputControls
{
	bool W = false;
	bool S = false;
	bool A = false;
	bool D = false;
};

void DrawFrame(GameConfig* gameConfig);

std::wstring map;
//HWND* Window;
HDC gameWindow;
GameConfig gameConfig;
InputControls input;
std::chrono::time_point<std::chrono::system_clock> tp1;
std::chrono::time_point<std::chrono::system_clock> tp2;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
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

	// Create Map of world space # = wall block, . = space
	map.empty();
	map += L"################";
	map += L"#..............#";
	map += L"#.......########";
	map += L"#..............#";
	map += L"#......##......#";
	map += L"#......##......#";
	map += L"#..............#";
	map += L"###............#";
	map += L"##.............#";
	map += L"#......####..###";
	map += L"#......#.......#";
	map += L"#......#.......#";
	map += L"#..............#";
	map += L"#......#########";
	map += L"#..............#";
	map += L"################";

	// Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SIMPLEFPS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMPLEFPS));

    MSG msg;

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
			gameConfig.PlayerA -= (gameConfig.Speed * 0.75f) * fElapsedTime;
		}

		if (input.D)
		{
			gameConfig.PlayerA += (gameConfig.Speed * 0.75f) * fElapsedTime;
		}

		if (input.W)
		{
			gameConfig.PlayerX += sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			gameConfig.PlayerY += cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			if (map.c_str()[(int)gameConfig.PlayerX * gameConfig.MapWidth + (int)gameConfig.PlayerY] == '#')
			{
				gameConfig.PlayerX -= sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
				gameConfig.PlayerY -= cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			}
		}

		if (input.S)
		{
			gameConfig.PlayerX -= sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			gameConfig.PlayerY -= cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			if (map.c_str()[(int)gameConfig.PlayerX * gameConfig.MapWidth + (int)gameConfig.PlayerY] == '#')
			{
				gameConfig.PlayerX += sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
				gameConfig.PlayerY += cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			}
		}

		// Render
		DrawFrame(&gameConfig);
	}

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
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

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
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

   RECT rect;
   if (GetWindowRect(hWnd, &rect))
   {
	   int width = rect.right - rect.left;
	   int height = rect.bottom - rect.top;

	   gameConfig.ScreenWidth = width;
	   gameConfig.ScreenHeight = height;
   }

   gameWindow = GetDC(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
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

void DrawFrame(GameConfig* gameConfig)
{
	COLORREF *screen = (COLORREF*)calloc(gameConfig->ScreenWidth*gameConfig->ScreenHeight, sizeof(COLORREF));

	// Find a walls around
	for (int x = 0; x < gameConfig->ScreenWidth; x++)
	{
		// For each column, calculate the projected ray angle into world space
		float rayAngle = (gameConfig->PlayerA - gameConfig->FOV / 2.0f) + ((float)x / (float)gameConfig->ScreenWidth) * gameConfig->FOV;

		// Find distance to wall
		float stepSize = 0.1f;		  // Increment size for ray casting, decrease to increase										
		float distanceToWall = 0.0f; //                                      resolution

		bool bHitWall = false;		// Set when ray hits wall block
		bool bBoundary = false;		// Set when ray hits boundary between two wall blocks

		float eyeX = sinf(rayAngle); // Unit vector for ray in player space
		float eyeY = cosf(rayAngle);

		// Incrementally cast ray from player, along ray angle, testing for 
		// intersection with a block
		while (!bHitWall && distanceToWall < gameConfig->Depth)
		{
			distanceToWall += stepSize;
			int testX = (int)(gameConfig->PlayerX + eyeX * distanceToWall);
			int testY = (int)(gameConfig->PlayerY + eyeY * distanceToWall);

			// Test if ray is out of bounds
			if (testX < 0 || testX >= gameConfig->MapWidth || testY < 0 || testY >= gameConfig->MapHeight)
			{
				bHitWall = true;			// Just set distance to maximum depth
				distanceToWall = gameConfig->Depth;
			}
			else
			{
				// Ray is inbounds so test to see if the ray cell is a wall block
				if (map.c_str()[testX * gameConfig->MapWidth + testY] == '#')
				{
					// Ray has hit wall
					bHitWall = true;

					// To highlight tile boundaries, cast a ray from each corner
					// of the tile, to the player. The more coincident this ray
					// is to the rendering ray, the closer we are to a tile 
					// boundary, which we'll shade to add detail to the walls
					std::vector<std::pair<float, float>> p;

					// Test each corner of hit tile, storing the distance from
					// the player, and the calculated dot product of the two rays
					for (int tx = 0; tx < 2; tx++)
						for (int ty = 0; ty < 2; ty++)
						{
							// Angle of corner to eye
							float vy = (float)testY + ty - gameConfig->PlayerY;
							float vx = (float)testX + tx - gameConfig->PlayerX;
							float d = sqrt(vx*vx + vy * vy);
							float dot = (eyeX * vx / d) + (eyeY * vy / d);
							p.push_back(std::make_pair(d, dot));
						}

					// Sort Pairs from closest to farthest
					sort(p.begin(), p.end(), [](const std::pair<float, float> &left, const std::pair<float, float> &right) {return left.first < right.first; });

					// First two/three are closest (we will never see all four)
					float bound = 0.01f;					
					bBoundary = acos(p.at(0).second) < bound || 
						acos(p.at(1).second) < bound || 
						acos(p.at(2).second) < bound;
				}
			}
		}

		// Calculate distance to ceiling and floor
		int nCeiling = floor((float)(gameConfig->ScreenHeight / 2.0) - gameConfig->ScreenHeight / ((float)distanceToWall));
		int nFloor = gameConfig->ScreenHeight - nCeiling;

		// Shader walls based on distance
		// Also funny thing about color:
		// the order is backward so RGB is BGR
		COLORREF nShade = RGB(0, 0, 0);
		if (distanceToWall <= gameConfig->Depth / 4.0f)			nShade = RGB(156, 0, 10);	// Very close	
		else if (distanceToWall < gameConfig->Depth / 3.0f)		nShade = RGB(133, 0, 9);
		else if (distanceToWall < gameConfig->Depth / 2.0f)		nShade = RGB(102, 0, 7);
		else if (distanceToWall < gameConfig->Depth)			nShade = RGB(79, 0, 5);
		else											nShade = RGB(54, 0, 4);		// Too far away

		if (bBoundary)
		{
			nShade = RGB(0, 0, 0);	// Black
		}

		for (int y = 0; y < gameConfig->ScreenHeight; y++)
		{
			COLORREF& pixel = screen[y*gameConfig->ScreenWidth + x];

			// Each Row
			if (y <= nCeiling)
			{
				pixel = RGB(0, 0, 0);
			}
			else if (y > nCeiling && y <= nFloor)
			{
				pixel = nShade;
			}
			else // Floor
			{
				// Shade floor based on distance
				float b = 1.0f - (((float)y - gameConfig->ScreenHeight / 2.0f) / ((float)gameConfig->ScreenHeight / 2.0f));
				if (b < 0.25)		nShade = RGB(110, 110, 110);
				else if (b < 0.5)	nShade = RGB(110, 110, 110);
				else if (b < 0.75)	nShade = RGB(90, 90, 90);
				else if (b < 0.9)	nShade = RGB(70, 70, 70);
				else				nShade = RGB(50, 50, 50);
				pixel = nShade;
			}
		}
	}

	// Display Frame
	screen[gameConfig->ScreenWidth * gameConfig->ScreenHeight - 1] = '\0';

	HBITMAP bitmap = CreateBitmap(gameConfig->ScreenWidth, gameConfig->ScreenHeight,
		1,
		8 * 4,
		(void*)screen);
				
	HDC src = CreateCompatibleDC(gameWindow);
	SelectObject(src, bitmap);
		
	BitBlt(gameWindow, // Destination
		0,  // x and
		0,  // y - upper-left corner of place, where we'd like to copy
		gameConfig->ScreenWidth,  // width of the region
		gameConfig->ScreenHeight, // height
		src, // source
		0,   // x and
		0,   // y of upper left corner  of part of the source, from where we'd like to copy
		SRCCOPY); // Defined DWORD to juct copy pixels. Watch more on msdn;

	DeleteDC(src); // Deleting temp HDC

	delete[]screen;
}
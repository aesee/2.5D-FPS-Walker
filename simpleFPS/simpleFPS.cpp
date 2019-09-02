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
	float Speed = 2.5f;			// Walking Speed
};

void DrawFrame(HDC hdc, GameConfig* gameConfig);

std::wstring map;
HWND* Window;
GameConfig gameConfig;
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

	

	
	
	//wchar_t *screen = new wchar_t[gameConfig.ScreenWidth*gameConfig.ScreenHeight];
	//HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); // How can I change it?
	//SetConsoleActiveScreenBuffer(hConsole);
	

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

	//std::chrono::time_point<std::chrono::system_clock> tp1 = std::chrono::system_clock::now();
	//std::chrono::time_point<std::chrono::system_clock> tp2 = std::chrono::system_clock::now();

	// Main cycle
	/*while (true)
	{
		// We'll need time differential per frame to calculate modification
		// to movement speeds, to ensure consistant movement, as ray-tracing
		// is non-deterministic
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		// Catch controls
		// Handle CCW Rotation
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			gameConfig.PlayerA -= (gameConfig.Speed * 0.75f) * fElapsedTime;

		// Handle CW Rotation
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			gameConfig.PlayerA += (gameConfig.Speed * 0.75f) * fElapsedTime;

		// Handle Forwards movement & collision
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			gameConfig.PlayerX += sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			gameConfig.PlayerY += cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			if (map.c_str()[(int)gameConfig.PlayerX * gameConfig.MapWidth + (int)gameConfig.PlayerY] == '#')
			{
				gameConfig.PlayerX -= sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
				gameConfig.PlayerY -= cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			}
		}

		// Handle backwards movement & collision
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			gameConfig.PlayerX -= sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			gameConfig.PlayerY -= cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			if (map.c_str()[(int)gameConfig.PlayerX * gameConfig.MapWidth + (int)gameConfig.PlayerY] == '#')
			{
				gameConfig.PlayerX += sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
				gameConfig.PlayerY += cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			}
		}

		

		// Display Stats
		//swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", gameConfig.PlayerX, gameConfig.PlayerY, gameConfig.PlayerA, 1.0f / fElapsedTime);

		// Display Map
		/ *for (int nx = 0; nx < gameConfig.MapWidth; nx++)
		{
			for (int ny = 0; ny < gameConfig.MapWidth; ny++)
			{
				screen[(ny + 1)*gameConfig.ScreenWidth + nx] = map[ny * gameConfig.MapWidth + nx];
			}
		}
		screen[((int)gameConfig.PlayerX + 1) * gameConfig.ScreenWidth + (int)gameConfig.PlayerY] = 'P';* /

		
		//WriteConsoleOutputCharacter(hConsole, screen, gameConfig.ScreenWidth * gameConfig.ScreenHeight, { 0,0 }, &dwBytesWritten);

		
	}*/

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

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		//DrawFrame(GetDC(*Window), &gameConfig);
    }

	/*HWND hWnd;
	hWnd = CreateWindowEx(NULL, //style stuff
		L"WindowClass", //name of the class
		L"CHIP-8", //tytle
		WS_OVERLAPPEDWINDOW, //options of window
		300, //coord x
		300, //coord y
		wr.right - wr.left, //width
		wr.bottom - wr.top, //height
		NULL, //window parent
		NULL, //menu bar handler
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);*/

	

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
	tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	tp1 = tp2;
	float fElapsedTime = elapsedTime.count();

	static float FPSTimer = 0;
	FPSTimer += fElapsedTime;

	if (FPSTimer > 1.0f / 30)
	{
		DrawFrame(GetDC(hWnd), &gameConfig);
		FPSTimer = 0;
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

	case WM_KEYDOWN: {
		switch (wParam)
		{
		case 'A': {
			gameConfig.PlayerA -= (gameConfig.Speed * 0.75f) * fElapsedTime;
			break;
		}
		case 'D': {
			gameConfig.PlayerA += (gameConfig.Speed * 0.75f) * fElapsedTime;
			break;
		}
		case 'W': {
			gameConfig.PlayerX += sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			gameConfig.PlayerY += cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			if (map.c_str()[(int)gameConfig.PlayerX * gameConfig.MapWidth + (int)gameConfig.PlayerY] == '#')
			{
				gameConfig.PlayerX -= sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
				gameConfig.PlayerY -= cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			}
			break;
		}
		case 'S': {
			gameConfig.PlayerX -= sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			gameConfig.PlayerY -= cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			if (map.c_str()[(int)gameConfig.PlayerX * gameConfig.MapWidth + (int)gameConfig.PlayerY] == '#')
			{
				gameConfig.PlayerX += sinf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
				gameConfig.PlayerY += cosf(gameConfig.PlayerA) * gameConfig.Speed * fElapsedTime;;
			}
			break;
		}
		}
	}


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

void DrawFrame(HDC hdc, GameConfig* gameConfig)
{
	COLORREF *screen = (COLORREF*)calloc(gameConfig->ScreenWidth*gameConfig->ScreenHeight, sizeof(COLORREF));

	// Find a walls around
	for (int x = 0; x < gameConfig->ScreenWidth; x++)
	{
		// For each column, calculate the projected ray angle into world space
		float fRayAngle = (gameConfig->PlayerA - gameConfig->FOV / 2.0f) + ((float)x / (float)gameConfig->ScreenWidth) * gameConfig->FOV;

		// Find distance to wall
		float fStepSize = 0.1f;		  // Increment size for ray casting, decrease to increase										
		float fDistanceToWall = 0.0f; //                                      resolution

		bool bHitWall = false;		// Set when ray hits wall block
		bool bBoundary = false;		// Set when ray hits boundary between two wall blocks

		float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
		float fEyeY = cosf(fRayAngle);

		// Incrementally cast ray from player, along ray angle, testing for 
		// intersection with a block
		while (!bHitWall && fDistanceToWall < gameConfig->Depth)
		{
			fDistanceToWall += fStepSize;
			int nTestX = (int)(gameConfig->PlayerX + fEyeX * fDistanceToWall);
			int nTestY = (int)(gameConfig->PlayerY + fEyeY * fDistanceToWall);

			// Test if ray is out of bounds
			if (nTestX < 0 || nTestX >= gameConfig->MapWidth || nTestY < 0 || nTestY >= gameConfig->MapHeight)
			{
				bHitWall = true;			// Just set distance to maximum depth
				fDistanceToWall = gameConfig->Depth;
			}
			else
			{
				// Ray is inbounds so test to see if the ray cell is a wall block
				if (map.c_str()[nTestX * gameConfig->MapWidth + nTestY] == '#')
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
							float vy = (float)nTestY + ty - gameConfig->PlayerY;
							float vx = (float)nTestX + tx - gameConfig->PlayerX;
							float d = sqrt(vx*vx + vy * vy);
							float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
							p.push_back(std::make_pair(d, dot));
						}

					// Sort Pairs from closest to farthest
					sort(p.begin(), p.end(), [](const std::pair<float, float> &left, const std::pair<float, float> &right) {return left.first < right.first; });

					// First two/three are closest (we will never see all four)
					float fBound = 0.01f;
					if (acos(p.at(0).second) < fBound) bBoundary = true;
					if (acos(p.at(1).second) < fBound) bBoundary = true;
					if (acos(p.at(2).second) < fBound) bBoundary = true;
				}
			}
		}

		// Calculate distance to ceiling and floor
		int nCeiling = floor((float)(gameConfig->ScreenHeight / 2.0) - gameConfig->ScreenHeight / ((float)fDistanceToWall));
		int nFloor = gameConfig->ScreenHeight - nCeiling;

		// Shader walls based on distance
		//short nShade = ' ';
		short nShade = RGB(20, 20, 20);
		if (fDistanceToWall <= gameConfig->Depth / 4.0f)			nShade = 0x2588;	// Very close	
		else if (fDistanceToWall < gameConfig->Depth / 3.0f)		nShade = 0x2593;
		else if (fDistanceToWall < gameConfig->Depth / 2.0f)		nShade = 0x2592;
		else if (fDistanceToWall < gameConfig->Depth)				nShade = 0x2591;
		else											nShade = RGB(20, 20, 20);		// Too far away

		if (bBoundary)
		{
			//nShade = ' '; // Black it out
			nShade = RGB(20, 20, 20);
		}

		for (int y = 0; y < gameConfig->ScreenHeight; y++)
		{
			// Each Row
			if (y <= nCeiling)
			{
				//screen[y*gameConfig.ScreenWidth + x] = ' ';
				screen[y*gameConfig->ScreenWidth + x] = RGB(0, 0, 0);
			}
			else if (y > nCeiling && y <= nFloor)
			{
				screen[y*gameConfig->ScreenWidth + x] = nShade;
			}
			else // Floor
			{
				// Shade floor based on distance
				float b = 1.0f - (((float)y - gameConfig->ScreenHeight / 2.0f) / ((float)gameConfig->ScreenHeight / 2.0f));
				if (b < 0.25)		nShade = RGB(110, 110, 110);//nShade = '#';
				else if (b < 0.5)	nShade = RGB(110, 110, 110);//nShade = 'x';
				else if (b < 0.75)	nShade = RGB(90, 90, 90);//nShade = '.';
				else if (b < 0.9)	nShade = RGB(70, 70, 70);//nShade = '-';
				else				nShade = RGB(50, 50, 50);//nShade = ' ';
				screen[y*gameConfig->ScreenWidth + x] = nShade;
			}
		}
	}

	// Display Frame
	screen[gameConfig->ScreenWidth * gameConfig->ScreenHeight - 1] = '\0';

	HBITMAP bitmap = CreateBitmap(gameConfig->ScreenWidth, gameConfig->ScreenHeight,
		1,
		8 * 4,
		(void*)screen);
	//HDC hdc = GetDC(nullptr);
	//HDC src = CreateCompatibleDC(hdc);
				
	HDC src = CreateCompatibleDC(hdc);
	SelectObject(src, bitmap);
		
	BitBlt(hdc, // Destination
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
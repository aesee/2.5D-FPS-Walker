#include "pch.h"
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>

#include <stdio.h>
#include <Windows.h>

struct GameConfig
{
	int ScreenWidth = 80;			// Console Screen Size X (columns)
	int ScreenHeight = 25;			// Console Screen Size Y (rows)
	int MapWidth = 16;				// World Dimensions
	int MapHeight = 16;

	float PlayerX = 14.7f;			// Player Start Position
	float PlayerY = 5.09f;
	float PlayerA = 0.0f;			// Player Start Rotation
	float FOV = 3.14159f / 4.0f;	// Field of View
	float Depth = 16.0f;			// Maximum rendering distance
	float Speed = 5.0f;			// Walking Speed
};


int main()
{
	GameConfig gameConfig;

	wchar_t *screen = new wchar_t[gameConfig.ScreenWidth*gameConfig.ScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); // How can I change it?
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Create Map of world space # = wall block, . = space
	std::wstring map;
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

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	// Main cycle
	while (true)
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

		// Find a walls around
		for (int x = 0; x < gameConfig.ScreenWidth; x++)
		{
			// For each column, calculate the projected ray angle into world space
			float fRayAngle = (gameConfig.PlayerA - gameConfig.FOV / 2.0f) + ((float)x / (float)gameConfig.ScreenWidth) * gameConfig.FOV;

			// Find distance to wall
			float fStepSize = 0.1f;		  // Increment size for ray casting, decrease to increase										
			float fDistanceToWall = 0.0f; //                                      resolution

			bool bHitWall = false;		// Set when ray hits wall block
			bool bBoundary = false;		// Set when ray hits boundary between two wall blocks

			float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
			float fEyeY = cosf(fRayAngle);

			// Incrementally cast ray from player, along ray angle, testing for 
			// intersection with a block
			while (!bHitWall && fDistanceToWall < gameConfig.Depth)
			{
				fDistanceToWall += fStepSize;
				int nTestX = (int)(gameConfig.PlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(gameConfig.PlayerY + fEyeY * fDistanceToWall);

				// Test if ray is out of bounds
				if (nTestX < 0 || nTestX >= gameConfig.MapWidth || nTestY < 0 || nTestY >= gameConfig.MapHeight)
				{
					bHitWall = true;			// Just set distance to maximum depth
					fDistanceToWall = gameConfig.Depth;
				}
				else
				{
					// Ray is inbounds so test to see if the ray cell is a wall block
					if (map.c_str()[nTestX * gameConfig.MapWidth + nTestY] == '#')
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
								float vy = (float)nTestY + ty - gameConfig.PlayerY;
								float vx = (float)nTestX + tx - gameConfig.PlayerX;
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
			int nCeiling = floor((float)(gameConfig.ScreenHeight / 2.0) - gameConfig.ScreenHeight / ((float)fDistanceToWall));
			int nFloor = gameConfig.ScreenHeight - nCeiling;

			// Shader walls based on distance
			short nShade = ' ';
			if (fDistanceToWall <= gameConfig.Depth / 4.0f)			nShade = 0x2588;	// Very close	
			else if (fDistanceToWall < gameConfig.Depth / 3.0f)		nShade = 0x2593;
			else if (fDistanceToWall < gameConfig.Depth / 2.0f)		nShade = 0x2592;
			else if (fDistanceToWall < gameConfig.Depth)				nShade = 0x2591;
			else											nShade = ' ';		// Too far away

			if (bBoundary)
			{
				nShade = ' '; // Black it out
			}

			for (int y = 0; y < gameConfig.ScreenHeight; y++)
			{
				// Each Row
				if (y <= nCeiling)
				{
					screen[y*gameConfig.ScreenWidth + x] = ' ';
				}
				else if (y > nCeiling && y <= nFloor)
				{
					screen[y*gameConfig.ScreenWidth + x] = nShade;
				}
				else // Floor
				{
					// Shade floor based on distance
					float b = 1.0f - (((float)y - gameConfig.ScreenHeight / 2.0f) / ((float)gameConfig.ScreenHeight / 2.0f));
					if (b < 0.25)		nShade = '#';
					else if (b < 0.5)	nShade = 'x';
					else if (b < 0.75)	nShade = '.';
					else if (b < 0.9)	nShade = '-';
					else				nShade = ' ';
					screen[y*gameConfig.ScreenWidth + x] = nShade;
				}
			}
		}

		// Display Stats
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", gameConfig.PlayerX, gameConfig.PlayerY, gameConfig.PlayerA, 1.0f / fElapsedTime);

		// Display Map
		for (int nx = 0; nx < gameConfig.MapWidth; nx++)
		{
			for (int ny = 0; ny < gameConfig.MapWidth; ny++)
			{
				screen[(ny + 1)*gameConfig.ScreenWidth + nx] = map[ny * gameConfig.MapWidth + nx];
			}
		}
		screen[((int)gameConfig.PlayerX + 1) * gameConfig.ScreenWidth + (int)gameConfig.PlayerY] = 'P';

		// Display Frame
		screen[gameConfig.ScreenWidth * gameConfig.ScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, gameConfig.ScreenWidth * gameConfig.ScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}
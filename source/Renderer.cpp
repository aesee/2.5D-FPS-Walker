#include "Renderer.h"
#include "Map.h"
#include <iostream>
#include <vector>
#include <algorithm>

void Renderer::SetScreenSize(int width, int height)
{
	m_resolution.x = width;
	m_resolution.y = height;
}

void Renderer::DrawFrame(Vector2D playerPosition, float playerRotation)
{
	CreateScreenBuffer();

	// Find a walls around
	for (int x = 0; x < m_resolution.x; x++)
	{
		// For each column, calculate the projected ray angle into world space
		float rayAngle = (playerRotation - m_fov / 2.0f) + ((float)x / m_resolution.x) * m_fov;

		// Find distance to wall
		float stepSize = 0.1f;		  // Increment size for ray casting, decrease to increase										
		float distanceToWall = 0.0f; //                                      resolution

		bool bHitWall = false;		// Set when ray hits wall block
		bool bBoundary = false;		// Set when ray hits boundary between two wall blocks

		float eyeX = sinf(rayAngle); // Unit vector for ray in player space
		float eyeY = cosf(rayAngle);

		// Incrementally cast ray from player, along ray angle, testing for 
		// intersection with a block
		while (!bHitWall && distanceToWall < m_depth)
		{
			distanceToWall += stepSize;
			int testX = (int)(playerPosition.x + eyeX * distanceToWall);
			int testY = (int)(playerPosition.y + eyeY * distanceToWall);

			// Test if ray is out of bounds
			if (testX < 0 || testX >= m_levelMap->GetSize().x || testY < 0 || testY >= m_levelMap->GetSize().y)
			{
				bHitWall = true;			// Just set distance to maximum depth
				distanceToWall = m_depth;
			}
			else
			{
				// Ray is inbounds so test to see if the ray cell is a wall block
				if (m_levelMap->IsWallIn(testX, testY))
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
							float vy = (float)testY + ty - playerPosition.y;
							float vx = (float)testX + tx - playerPosition.x;
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
		int nCeiling = (int) floor(m_resolution.y / 2.0 - m_resolution.y / distanceToWall);
		int nFloor = m_resolution.y - nCeiling;

		// Shader walls based on distance
		// Also funny thing about color:
		// the order is backward so RGB is BGR
		COLORREF nShade = RGB(0, 0, 0);
		if (distanceToWall <= m_depth / 4.0f)			nShade = RGB(156, 0, 10);	// Very close	
		else if (distanceToWall < m_depth / 3.0f)		nShade = RGB(133, 0, 9);
		else if (distanceToWall < m_depth / 2.0f)		nShade = RGB(102, 0, 7);
		else if (distanceToWall < m_depth)			nShade = RGB(79, 0, 5);
		else											nShade = RGB(54, 0, 4);		// Too far away

		if (bBoundary)
		{
			nShade = RGB(0, 0, 0);	// Black
		}

		for (int y = 0; y < m_resolution.y; y++)
		{
			int pixel = static_cast<int>(y*m_resolution.x + x);

			// Each Row
			if (y <= nCeiling)
			{
				SetPixel(pixel, RGB(0, 0, 0));
			}
			else if (y > nCeiling && y <= nFloor)
			{
				SetPixel(pixel, nShade);
			}
			else // Floor
			{
				// Shade floor based on distance
				float b = 1.0f - (((float)y - m_resolution.y / 2.0f) / (m_resolution.y / 2.0f));
				if (b < 0.25)		nShade = RGB(110, 110, 110);
				else if (b < 0.5)	nShade = RGB(110, 110, 110);
				else if (b < 0.75)	nShade = RGB(90, 90, 90);
				else if (b < 0.9)	nShade = RGB(70, 70, 70);
				else				nShade = RGB(50, 50, 50);
				SetPixel(pixel, nShade);
			}
		}
	}

	// Display Frame
	MoveBufferToScreen();
}

COLORREF* Renderer::CreateScreenBuffer()
{
	m_screen = new COLORREF[m_resolution.x * m_resolution.y];
	return m_screen;
}

void Renderer::SetPixel(INT32 pixel, COLORREF value)
{
	m_screen[pixel] = value;
}

void Renderer::MoveBufferToScreen()
{
	SetPixel(static_cast<int>(m_resolution.x * m_resolution.y - 1), '\0');

	HBITMAP bitmap = CreateBitmap(m_resolution.x, m_resolution.y,
		1,
		8 * 4,
		(void*)m_screen);

	HDC src = CreateCompatibleDC(m_gameWindow);
	SelectObject(src, bitmap);

	BitBlt(m_gameWindow, // Destination
		0,  // x and
		0,  // y - upper-left corner of place, where we'd like to copy
		m_resolution.x,  // width of the region
		m_resolution.y, // height
		src, // source
		0,   // x and
		0,   // y of upper left corner  of part of the source, from where we'd like to copy
		SRCCOPY); // Defined DWORD to juct copy pixels. Watch more on msdn;

	DeleteDC(src); // Deleting temp HDC

	delete[] m_screen;
	m_screen = nullptr;
}
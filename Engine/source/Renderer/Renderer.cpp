#include "Renderer.h"
#include "Map.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "Colors.h"
#include <cmath>

Renderer::Renderer(HDC window, Vector2 fov, float depth)
	: m_gameWindow(window), m_fov(fov), m_depth(depth)
{ }

Renderer::~Renderer()
{
	delete[] m_screen;
}

void Renderer::SetScreenSize(int width, int height)
{
	m_resolution.x = width;
	m_resolution.y = height;

	m_screen = new COLORREF[m_resolution.x * m_resolution.y];
}

void Renderer::SetScreenSize(IntVector2D& screenSize)
{
	m_resolution.x = screenSize.x;
	m_resolution.y = screenSize.y;

	m_screen = new COLORREF[m_resolution.x * m_resolution.y];
}

void Renderer::DrawFrame(Vector3& playerPosition, float playerRotation)
{
	const int NumSectors = 16;
	float playerYaw = 0;

	//////

	enum { MaxQueue = 32 };  // maximum number of pending portal renders
	NodeSector queue[MaxQueue];
	NodeSector* head = queue;
	NodeSector* tail = queue;

	std::vector<int> ytop(GetScreenSize().x);
	std::fill(ytop.begin(), ytop.end(), 0);

	std::vector<int> ybottom(GetScreenSize().x);
	std::fill(ybottom.begin(), ybottom.end(), GetScreenSize().y - 1);

	std::vector<int> renderedsectors(NumSectors);
	std::fill(renderedsectors.begin(), renderedsectors.end(), 0);

	// Find current sector
	CheckCurrentSector(playerPosition);

	/* Begin whole-screen rendering from where the player is. */
	*head = { 
		m_currentRenderingSector,
		0, 
		GetScreenSize().x - 1 };

	if (++head == queue + MaxQueue)
	{
		head = queue;
	}

	do {
		/* Pick a sector & slice from the queue to draw */
		const NodeSector currentNode = *tail;
		if (++tail == queue + MaxQueue)
		{
			tail = queue;
		}

		if (currentNode.sectorno > renderedsectors.size() - 1)
		{
			continue;
		}
		++renderedsectors[currentNode.sectorno];
		const Sector* const sector = &m_levelMap->sectors[currentNode.sectorno];
		/* Render each wall of this sector that is facing towards player. */
		for (unsigned s = 0; s < sector->npoints; ++s)
		{
			// Acquire transformed coordinates of two endpoints (vertices) of this edge of the sector 
			Vector2 transformedPoint1, transformedPoint2;
			{
				// Acquire coordinates of original endpoints (vertices)
				Vector2 endpoint1(sector->vertex[s + 0].x - playerPosition.x, sector->vertex[s + 0].y - playerPosition.y);
				Vector2 endpoint2(sector->vertex[s + 1].x - playerPosition.x, sector->vertex[s + 1].y - playerPosition.y);
				/* Rotate them around the player's view */
				float pcos = std::cos(playerRotation);
				float psin = std::sin(playerRotation);

				transformedPoint1 = Vector2(endpoint1.x * psin - endpoint1.y * pcos, endpoint1.x * pcos + endpoint1.y * psin);
				transformedPoint2 = Vector2(endpoint2.x * psin - endpoint2.y * pcos, endpoint2.x * pcos + endpoint2.y * psin);
			}
			/* Is the wall at least partially in front of the player? */
			if (transformedPoint1.y <= 0 && transformedPoint2.y <= 0)
			{
				continue;
			}
			/* If it's partially behind the player, clip it against player's view frustrum */
			if (transformedPoint1.y <= 0 || transformedPoint2.y <= 0)
			{
				auto Intersect = [](Vector2& v1, Vector2& v2, Vector2& v3, Vector2& v4)
				{
					Vector2 vector1x = Vector2(v1.CrossProduct(v2), v1.x - v2.x);
					Vector2 vector2x = Vector2(v3.CrossProduct(v4), v3.x - v4.x);
					Vector2 vector1y = Vector2(v1.CrossProduct(v2), v1.y - v2.y);
					Vector2 vector2y = Vector2(v3.CrossProduct(v4), v3.y - v4.y);

					return Vector2(vector1x.CrossProduct(vector2x) / (v1 - v2).CrossProduct(v3 - v4),
						vector1y.CrossProduct(vector2y) / (v1 - v2).CrossProduct(v3 - v4));
				};
				
				// Find an intersection between the wall and the approximate edges of player's view
				{
					float nearz = 1e-4f, farz = 5, nearside = 1e-5f, farside = 20.f;
					Vector2 nearside1(-nearside, nearz);
					Vector2 farside1(-farside, farz);
					Vector2 nearside2(nearside, nearz);
					Vector2 farside2(farside, farz);

					Vector2 intersection1 = Intersect(transformedPoint1, transformedPoint2, nearside1, farside1);
					Vector2 intersection2 = Intersect(transformedPoint1, transformedPoint2, nearside2, farside2);
					if (transformedPoint1.y < nearz)
					{
						transformedPoint1 = intersection1.y > 0 ? intersection1 : intersection2;
					}
					if (transformedPoint2.y < nearz)
					{
						transformedPoint2 = intersection1.y > 0 ? intersection1 : intersection2;
					}
				}
			}
			// Project onto the screen
			Vector2 scale1(m_fov.x / transformedPoint1.y, m_fov.y / transformedPoint1.y);
			int projection1 = GetScreenSize().x / 2 - (int)(transformedPoint1.x * scale1.x);
			Vector2 scale2(m_fov.x / transformedPoint2.y, m_fov.y / transformedPoint2.y);
			int projection2 = GetScreenSize().x / 2 - (int)(transformedPoint2.x * scale2.x);
			if (projection1 >= projection2 || projection2 < currentNode.sx1 || projection1 > currentNode.sx2) 
			{
				continue; // render if it's visible
			}
			/* Acquire the floor and ceiling heights, relative to where the player's view is */
			float ceilHeight = sector->ceil - playerPosition.z;
			float floorHeight = sector->floor - playerPosition.z;
			/* Check the edge type. neighbor=-1 means wall, other=boundary between two sectors. */
			int neighbor = sector->neighbors[s];
			float neighborCeilHeight = 0; 
			float neighborFloorHeight = 0;
			if (neighbor >= 0) // Is another sector showing through this portal?
			{
				neighborCeilHeight = m_levelMap->sectors[neighbor].ceil - playerPosition.z;
				neighborFloorHeight = m_levelMap->sectors[neighbor].floor - playerPosition.z;
			}
			/* Project our ceiling & floor heights into screen coordinates (Y coordinate) */
			auto GetYaw = [&playerYaw](float y, float z)
			{
				return static_cast<int> (y + z * playerYaw);
			};

			int y1a = GetScreenSize().y / 2 - (GetYaw(ceilHeight, transformedPoint1.y) * scale1.y);
			int	y1b = GetScreenSize().y / 2 - (GetYaw(floorHeight, transformedPoint1.y) * scale1.y);
			int y2a = GetScreenSize().y / 2 - (GetYaw(ceilHeight, transformedPoint2.y) * scale2.y);
			int y2b = GetScreenSize().y / 2 - (GetYaw(floorHeight, transformedPoint2.y) * scale2.y);
			/* The same for the neighboring sector */
			int ny1a = GetScreenSize().y / 2 - (GetYaw(neighborCeilHeight, transformedPoint1.y) * scale1.y);
			int ny1b = GetScreenSize().y / 2 - (GetYaw(neighborFloorHeight, transformedPoint1.y) * scale1.y);
			int ny2a = GetScreenSize().y / 2 - (GetYaw(neighborCeilHeight, transformedPoint2.y) * scale2.y);
			int ny2b = GetScreenSize().y / 2 - (GetYaw(neighborFloorHeight, transformedPoint2.y) * scale2.y);

			/* Render the wall. */
			int beginx = std::max(projection1, currentNode.sx1);
			int endx = std::min(projection2, currentNode.sx2);
			for (int x = beginx; x <= endx; ++x)
			{
				/* Calculate the Z coordinate for this point. (Only used for lighting.) */
				int z = ((x - projection1) * (transformedPoint2.y - transformedPoint1.y) / (projection2 - projection1) + transformedPoint1.y) * 8;
				/* Acquire the Y coordinates for our ceiling & floor for this X coordinate. Clamp them. */
				int ya = (x - projection1) * (y2a - y1a) / (projection2 - projection1) + y1a;
				int cya = std::clamp(ya, ytop[x], ybottom[x]); // top
				int yb = (x - projection1) * (y2b - y1b) / (projection2 - projection1) + y1b;
				int cyb = std::clamp(yb, ytop[x], ybottom[x]); // bottom

				/* Render ceiling: everything above this sector's ceiling height. */
				DrawVerticalLine(x, ytop[x], cya - 1, Colors::Black, Colors::Nero, Colors::Black);
				/* Render floor: everything below this sector's floor height. */
				DrawVerticalLine(x, cyb + 1, ybottom[x], Colors::Blue, Colors::MidnightBlue, Colors::Blue);

				/* Is there another sector behind this edge? */
				if (neighbor >= 0)
				{
					/* Same for _their_ floor and ceiling */
					int nya = (x - projection1) * (ny2a - ny1a) / (projection2 - projection1) + ny1a, cnya = std::clamp(nya, ytop[x], ybottom[x]);
					int nyb = (x - projection1) * (ny2b - ny1b) / (projection2 - projection1) + ny1b, cnyb = std::clamp(nyb, ytop[x], ybottom[x]);
					/* If our ceiling is higher than their ceiling, render upper wall */
					unsigned r1 = Colors::White * (255 - z);
					unsigned r2 = Colors::BlackCurrant * (31 - z / 8);
					DrawVerticalLine(x, cya, cnya - 1, 0, x == projection1 || x == projection2 ? 0 : r1, 0); // Between our and their ceiling
					ytop[x] = std::clamp(std::max(cya, cnya), ytop[x], GetScreenSize().y - 1);   // Shrink the remaining window below these ceilings
					/* If our floor is lower than their floor, render bottom wall */
					DrawVerticalLine(x, cnyb + 1, cyb, 0, x == projection1 || x == projection2 ? 0 : r2, 0); // Between their and our floor
					ybottom[x] = std::clamp(std::min(cyb, cnyb), 0, ybottom[x]); // Shrink the remaining window above these floors
				}
				else
				{
					/* There's no neighbor. Render wall from top (cya = ceiling level) to bottom (cyb = floor level). */
					unsigned r = Colors::White * (255 - z);
					DrawVerticalLine(x, cya, cyb, 0, x == projection1 || x == projection2 ? 0 : r, 0);
				}
			}
			/* Schedule the neighboring sector for rendering within the window formed by this wall. */
			if (neighbor >= 0 && endx >= beginx && (head + MaxQueue + 1 - tail) % MaxQueue)
			{
				*head = 
				{ 
					neighbor, 
					beginx, 
					endx 
				};
				if (++head == queue + MaxQueue)
				{
					head = queue;
				}
			}
		} // for s in sector's edges
		++renderedsectors[currentNode.sectorno];
	} while (head != tail); // render any other queued sectors

	// Display Frame
	MoveBufferToScreen();
}

/*COLORREF* Renderer::CreateScreenBuffer()
{
	m_screen = new COLORREF[m_resolution.x * m_resolution.y];
	return m_screen;
}*/

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
	DeleteObject(bitmap);
}

void Renderer::DrawVerticalLine(int x, int y1, int y2, int top, int middle, int bottom)
{
	IntVector2D resolution = GetScreenSize();

	y1 = std::clamp(y1, 0, resolution.y - 1);
	y2 = std::clamp(y2, 0, resolution.y - 1);
	if (y2 == y1)
	{
		SetPixel(y1*resolution.x + x, middle);
	}
	else if (y2 > y1)
	{
		SetPixel(y1*resolution.x + x, top);
		for (int y = y1 + 1; y < y2; ++y)
		{
			SetPixel(y*resolution.x + x, middle);
		}
		SetPixel(y2*resolution.x + x, bottom);
	}
}

void Renderer::CheckCurrentSector(Vector3& playerPosition)
{
	const Sector* const currentSector = &m_levelMap->sectors[m_currentRenderingSector];
	const Vector2* const vert = currentSector->vertex;
	Vector2 playerPosition2D(playerPosition.x, playerPosition.y);

	auto IsPointInBox = [](Vector2& playerPosition, Vector2& boxPoint1, Vector2& boxPoint2)
	{
		return (playerPosition.x < std::max(boxPoint1.x, boxPoint2.x)) &&
			(playerPosition.y < std::max(boxPoint1.y, boxPoint2.y)) &&
			(std::min(boxPoint1.x, boxPoint2.x) < playerPosition.x) &&
			(std::min(boxPoint1.y, boxPoint2.y) < playerPosition.y);
	};

	auto IsPointOnSide = [](Vector2& playerPosition, Vector2& boxPoint1, Vector2& boxPoint2)
	{
		return (boxPoint2 - boxPoint1).CrossProduct(playerPosition - boxPoint1) < 0;
	};

	for (unsigned s = 0; s < currentSector->npoints; ++s)
	{
		Vector2 boxPoint1 = vert[s + 0];
		Vector2 boxPoint2 = vert[s + 1];

		if (currentSector->neighbors[s] >= 0
			&& IsPointInBox(playerPosition2D, boxPoint1, boxPoint2)
			&& IsPointOnSide(playerPosition2D, boxPoint1, boxPoint2))
		{
			m_currentRenderingSector = currentSector->neighbors[s];
			break;
		}
	}
}

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

	/*const Sector* const sector = &m_levelMap->sectors[m_currentRenderingSector];
	const Vector2* const vert = sector->vertex;
#define min(a,b)             (((a) < (b)) ? (a) : (b)) // min: Choose smaller of two scalars.
#define max(a,b)             (((a) > (b)) ? (a) : (b)) // max: Choose greater of two scalars.
#define Overlap(a0,a1,b0,b1) (min(a0,a1) <= max(b0,b1) && min(b0,b1) <= max(a0,a1))
#define vxs(x0,y0, x1,y1)    ((x0)*(y1) - (x1)*(y0))   // vxs: Vector cross product
#define IntersectBox(x0,y0, x1,y1, x2,y2, x3,y3) (Overlap(x0,x1,x2,x3) && Overlap(y0,y1,y2,y3))
#define PointSide(px,py, x0,y0, x1,y1) vxs((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0))
	float px = playerPosition.x;
	float py = playerPosition.y;
	for (unsigned s = 0; s < sector->npoints; ++s)
	{
		if (sector->neighbors[s] >= 0
			&& IntersectBox(px, py, px + dx, py + dy, vert[s + 0].x, vert[s + 0].y, vert[s + 1].x, vert[s + 1].y)
			&& PointSide(px + dx, py + dy, vert[s + 0].x, vert[s + 0].y, vert[s + 1].x, vert[s + 1].y) < 0)
		{
			m_currentRenderingSector = sector->neighbors[s];
			break;
		}
	}*/

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
			/* Acquire the x,y coordinates of the two endpoints (vertices) of this edge of the sector */
			Vector2 endpoint1(sector->vertex[s + 0].x - playerPosition.x, sector->vertex[s + 0].y - playerPosition.y);
			Vector2 endpoint2(sector->vertex[s + 1].x - playerPosition.x, sector->vertex[s + 1].y - playerPosition.y);
			/* Rotate them around the player's view */
			float pcos = std::cos(playerRotation);
			float psin = std::sin(playerRotation);

			Vector2 transformedPoint1(endpoint1.x * psin - endpoint1.y * pcos, endpoint1.x * pcos + endpoint1.y * psin);
			Vector2 transformedPoint2(endpoint2.x * psin - endpoint2.y * pcos, endpoint2.x * pcos + endpoint2.y * psin);
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
			/* Do perspective transformation */
			Vector2 scale1(m_fov.x / transformedPoint1.y, m_fov.y / transformedPoint1.y);
			int x1 = GetScreenSize().x / 2 - (int)(transformedPoint1.x * scale1.x);
			Vector2 scale2(m_fov.x / transformedPoint2.y, m_fov.y / transformedPoint2.y);
			int x2 = GetScreenSize().x / 2 - (int)(transformedPoint2.x * scale2.x);
			if (x1 >= x2 || x2 < currentNode.sx1 || x1 > currentNode.sx2) 
			{
				continue; // Only render if it's visible
			}
			/* Acquire the floor and ceiling heights, relative to where the player's view is */
			float yceil = sector->ceil - playerPosition.z;
			float yfloor = sector->floor - playerPosition.z;
			/* Check the edge type. neighbor=-1 means wall, other=boundary between two sectors. */
			int neighbor = sector->neighbors[s];
			float nyceil = 0; 
			float nyfloor = 0;
			if (neighbor >= 0) // Is another sector showing through this portal?
			{
				nyceil = m_levelMap->sectors[neighbor].ceil - playerPosition.z;
				nyfloor = m_levelMap->sectors[neighbor].floor - playerPosition.z;
			}
			/* Project our ceiling & floor heights into screen coordinates (Y coordinate) */
			auto GetYaw = [&playerYaw](float y, float z)
			{
				return static_cast<int> (y + z * playerYaw);
			};

			int y1a = GetScreenSize().y / 2 - (GetYaw(yceil, transformedPoint1.y) * scale1.y);
			int	y1b = GetScreenSize().y / 2 - (GetYaw(yfloor, transformedPoint1.y) * scale1.y);
			int y2a = GetScreenSize().y / 2 - (GetYaw(yceil, transformedPoint2.y) * scale2.y);
			int y2b = GetScreenSize().y / 2 - (GetYaw(yfloor, transformedPoint2.y) * scale2.y);
			/* The same for the neighboring sector */
			int ny1a = GetScreenSize().y / 2 - (GetYaw(nyceil, transformedPoint1.y) * scale1.y);
			int ny1b = GetScreenSize().y / 2 - (GetYaw(nyfloor, transformedPoint1.y) * scale1.y);
			int ny2a = GetScreenSize().y / 2 - (GetYaw(nyceil, transformedPoint2.y) * scale2.y);
			int ny2b = GetScreenSize().y / 2 - (GetYaw(nyfloor, transformedPoint2.y) * scale2.y);

			/* Render the wall. */
			int beginx = max(x1, currentNode.sx1);
			int endx = min(x2, currentNode.sx2);
			for (int x = beginx; x <= endx; ++x)
			{
				/* Calculate the Z coordinate for this point. (Only used for lighting.) */
				int z = ((x - x1) * (transformedPoint2.y - transformedPoint1.y) / (x2 - x1) + transformedPoint1.y) * 8;
				/* Acquire the Y coordinates for our ceiling & floor for this X coordinate. Clamp them. */
				int ya = (x - x1) * (y2a - y1a) / (x2 - x1) + y1a;
				int cya = std::clamp(ya, ytop[x], ybottom[x]); // top
				int yb = (x - x1) * (y2b - y1b) / (x2 - x1) + y1b;
				int cyb = std::clamp(yb, ytop[x], ybottom[x]); // bottom

				/* Render ceiling: everything above this sector's ceiling height. */
				DrawVerticalLine(x, ytop[x], cya - 1, Colors::Black, Colors::Nero, Colors::Black);
				/* Render floor: everything below this sector's floor height. */
				DrawVerticalLine(x, cyb + 1, ybottom[x], Colors::Blue, Colors::MidnightBlue, Colors::Blue);

				/* Is there another sector behind this edge? */
				if (neighbor >= 0)
				{
					/* Same for _their_ floor and ceiling */
					int nya = (x - x1) * (ny2a - ny1a) / (x2 - x1) + ny1a, cnya = std::clamp(nya, ytop[x], ybottom[x]);
					int nyb = (x - x1) * (ny2b - ny1b) / (x2 - x1) + ny1b, cnyb = std::clamp(nyb, ytop[x], ybottom[x]);
					/* If our ceiling is higher than their ceiling, render upper wall */
					unsigned r1 = Colors::White * (255 - z);
					unsigned r2 = Colors::BlackCurrant * (31 - z / 8);
					DrawVerticalLine(x, cya, cnya - 1, 0, x == x1 || x == x2 ? 0 : r1, 0); // Between our and their ceiling
					ytop[x] = std::clamp(max(cya, cnya), ytop[x], GetScreenSize().y - 1);   // Shrink the remaining window below these ceilings
					/* If our floor is lower than their floor, render bottom wall */
					DrawVerticalLine(x, cnyb + 1, cyb, 0, x == x1 || x == x2 ? 0 : r2, 0); // Between their and our floor
					ybottom[x] = std::clamp(min(cyb, cnyb), 0, ybottom[x]); // Shrink the remaining window above these floors
				}
				else
				{
					/* There's no neighbor. Render wall from top (cya = ceiling level) to bottom (cyb = floor level). */
					unsigned r = Colors::White * (255 - z);
					DrawVerticalLine(x, cya, cyb, 0, x == x1 || x == x2 ? 0 : r, 0);
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

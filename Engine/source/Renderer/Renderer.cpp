#include "Renderer.h"
#include "Map.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "Colors.h"
#include <cmath>

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
	/*//CreateScreenBuffer();

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
		int nCeiling = static_cast<int> (round(m_resolution.y / 2 - m_resolution.y / distanceToWall));
		int nFloor = m_resolution.y - nCeiling;

		// TODO: draw a different walls!
		// Shader walls based on distance
		// Also funny thing about color:
		// the order is backward so RGB is BGR
		COLORREF nShade = Colors::Black;
		if (distanceToWall <= m_depth / 4.0f)			nShade = Colors::Blue;	// Very close
		else if (distanceToWall < m_depth / 3.0f)		nShade = Colors::MediumBlue;
		else if (distanceToWall < m_depth / 2.0f)		nShade = Colors::DarkBlue;
		else if (distanceToWall < m_depth)			nShade = Colors::Navy;
		else											nShade = Colors::MidnightBlue;		// Too far away

		if (bBoundary)
		{
			nShade = Colors::Black;
		}

		for (int y = 0; y < m_resolution.y; y++)
		{
			int pixel = static_cast<int>(y*m_resolution.x + x);

			// Each Row
			if (y <= nCeiling)
			{
				SetPixel(pixel, Colors::Black);
			}
			else if (y > nCeiling && y <= nFloor)
			{
				SetPixel(pixel, nShade);
			}
			else // Floor
			{
				// Shade floor based on distance
				float b = 1.0f - (((float)y - m_resolution.y / 2.0f) / (m_resolution.y / 2.0f));
				if (b < 0.25)		nShade = Colors::Grey(5);
				else if (b < 0.5)	nShade = Colors::Grey(4);
				else if (b < 0.75)	nShade = Colors::Grey(3);
				else if (b < 0.9)	nShade = Colors::Grey(2);
				else				nShade = Colors::Grey(1);
				SetPixel(pixel, nShade);
			}
		}
	}

	// Display Frame
	MoveBufferToScreen();*/

	const int screenWidth = GetScreenSize().x;
	const int screenHeight = GetScreenSize().y;
	const int NumSectors = 16;

	const float hfov = 0.73f*screenHeight;
	const float vfov = 0.2f*screenHeight;

	float playerYaw = 0;

	//////

	enum { MaxQueue = 32 };  // maximum number of pending portal renders
	NodeSector queue[MaxQueue];
	NodeSector* head = queue;
	NodeSector* tail = queue;

	std::vector<int> ytop(screenWidth);
	std::fill(ytop.begin(), ytop.end(), 0);

	std::vector<int> ybottom(screenWidth);
	std::vector<int> renderedsectors(NumSectors);
	for (unsigned x = 0; x < screenWidth; ++x)
	{
		ybottom[x] = screenHeight - 1;
	}
	for (unsigned n = 0; n < NumSectors; ++n)
	{
		renderedsectors[n] = 0;
	}

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
		screenWidth - 1 };

	if (++head == queue + MaxQueue) head = queue;

	do {
		/* Pick a sector & slice from the queue to draw */
		const struct NodeSector now = *tail;
		if (++tail == queue + MaxQueue)
		{
			tail = queue;
		}

		if (renderedsectors[now.sectorno] & 0x21)
		{
			continue; // Odd = still rendering, 0x20 = give up
		}
		++renderedsectors[now.sectorno];
		const Sector* const sect = &m_levelMap->sectors[now.sectorno];
		/* Render each wall of this sector that is facing towards player. */
		for (unsigned s = 0; s < sect->npoints; ++s)
		{
			/* Acquire the x,y coordinates of the two endpoints (vertices) of this edge of the sector */
			float vx1 = sect->vertex[s + 0].x - playerPosition.x, vy1 = sect->vertex[s + 0].y - playerPosition.y;
			float vx2 = sect->vertex[s + 1].x - playerPosition.x, vy2 = sect->vertex[s + 1].y - playerPosition.y;
			/* Rotate them around the player's view */
			//float pcos = player.anglecos, psin = player.anglesin;
			float pcos = std::cos(playerRotation), psin = std::sin(playerRotation);
			float tx1 = vx1 * psin - vy1 * pcos, tz1 = vx1 * pcos + vy1 * psin;
			float tx2 = vx2 * psin - vy2 * pcos, tz2 = vx2 * pcos + vy2 * psin;
			/* Is the wall at least partially in front of the player? */
			if (tz1 <= 0 && tz2 <= 0)
			{
				continue;
			}
			/* If it's partially behind the player, clip it against player's view frustrum */
			if (tz1 <= 0 || tz2 <= 0)
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

				float nearz = 1e-4f, farz = 5, nearside = 1e-5f, farside = 20.f;
				// Find an intersection between the wall and the approximate edges of player's view
				Vector2 t1(tx1, tz1);
				Vector2 t2(tx2, tz2);
				Vector2 nearside1(-nearside, nearz);
				Vector2 farside1(-farside, farz);
				Vector2 nearside2(nearside, nearz);
				Vector2 farside2(farside, farz);

				Vector2 i1 = Intersect(t1, t2, nearside1, farside1);
				Vector2 i2 = Intersect(t1, t2, nearside2, farside2);
				if (tz1 < nearz) { if (i1.y > 0) { tx1 = i1.x; tz1 = i1.y; } else { tx1 = i2.x; tz1 = i2.y; } }
				if (tz2 < nearz) { if (i1.y > 0) { tx2 = i1.x; tz2 = i1.y; } else { tx2 = i2.x; tz2 = i2.y; } }
			}
			/* Do perspective transformation */
			float xscale1 = hfov / tz1, yscale1 = vfov / tz1;    int x1 = screenWidth / 2 - (int)(tx1 * xscale1);
			float xscale2 = hfov / tz2, yscale2 = vfov / tz2;    int x2 = screenWidth / 2 - (int)(tx2 * xscale2);
			if (x1 >= x2 || x2 < now.sx1 || x1 > now.sx2) continue; // Only render if it's visible
			/* Acquire the floor and ceiling heights, relative to where the player's view is */
			float yceil = sect->ceil - playerPosition.z;
			float yfloor = sect->floor - playerPosition.z;
			/* Check the edge type. neighbor=-1 means wall, other=boundary between two sectors. */
			int neighbor = sect->neighbors[s];
			float nyceil = 0, nyfloor = 0;
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

			int y1a = screenHeight / 2 - (GetYaw(yceil, tz1) * yscale1);
			int	y1b = screenHeight / 2 - (GetYaw(yfloor, tz1) * yscale1);
			int y2a = screenHeight / 2 - (GetYaw(yceil, tz2) * yscale2);
			int y2b = screenHeight / 2 - (GetYaw(yfloor, tz2) * yscale2);
			/* The same for the neighboring sector */
			int ny1a = screenHeight / 2 - (GetYaw(nyceil, tz1) * yscale1); 
			int ny1b = screenHeight / 2 - (GetYaw(nyfloor, tz1) * yscale1);
			int ny2a = screenHeight / 2 - (GetYaw(nyceil, tz2) * yscale2); 
			int ny2b = screenHeight / 2 - (GetYaw(nyfloor, tz2) * yscale2);

			/* Render the wall. */
			int beginx = max(x1, now.sx1), endx = min(x2, now.sx2);
			for (int x = beginx; x <= endx; ++x)
			{
				/* Calculate the Z coordinate for this point. (Only used for lighting.) */
				int z = ((x - x1) * (tz2 - tz1) / (x2 - x1) + tz1) * 8;
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
					ytop[x] = std::clamp(max(cya, cnya), ytop[x], screenHeight - 1);   // Shrink the remaining window below these ceilings
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
				if (++head == queue + MaxQueue) head = queue;
			}
		} // for s in sector's edges
		++renderedsectors[now.sectorno];
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

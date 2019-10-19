#pragma once
#include "EngineLib.h"
#include <Windows.h>

class Map;

class Renderer
{
public:
	Renderer(HDC window, float fov, float depth)
		: m_gameWindow(window), m_fov(fov), m_depth(depth) {}
	~Renderer();

	void SetScreenSize(int width, int height);
	void SetScreenSize(IntVector2D& screenSize);
	void SetMap(Map* map) { m_levelMap = map; }
	void DrawFrame(Vector3& playerPosition, float playerRotation);

private:
	//COLORREF* CreateScreenBuffer();
	IntVector2D GetScreenSize() { return m_resolution; }
	void SetPixel(INT32 pixel, COLORREF value);
	void MoveBufferToScreen();

	void DrawVerticalLine(int x, int y1, int y2, int top, int middle, int bottom);

private:
	IntVector2D m_resolution;
	COLORREF* m_screen;
	HDC m_gameWindow;
	Map* m_levelMap;
	float m_fov;
	float m_depth;
};

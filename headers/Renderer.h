#pragma once
#include "EngineLib.h"
#include <Windows.h>

class Map;

class Renderer
{
public:
	Renderer(HDC window, float fov, float depth) 
	{ 
		gameWindow = window; 
		this->fov = fov;
		this->depth = depth;
	}
	~Renderer() {}

	void SetScreenSize(int width, int height);
	void SetMap(Map* map) { levelMap = map; }
	void DrawFrame(Vector2D playerPosition, float playerRotation);

private:
	COLORREF* CreateScreenBuffer();
	IntVector2D GetScreenSize() { return resolution; }
	void SetPixel(INT32 pixel, COLORREF value);
	void MoveBufferToScreen();

private:
	IntVector2D resolution = IntVector2D(800, 600);
	COLORREF* screen;
	HDC gameWindow;
	Map* levelMap;
	float fov;
	float depth;
};

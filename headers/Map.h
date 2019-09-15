#pragma once
#include "EngineLib.h"
#include <iostream>

class Map
{
public:
	Map();
	~Map();
	Vector2D GetSize() { return mapSize; }
	bool IsWallIn(int x, int y);

private:
	Vector2D mapSize = Vector2D{ 16, 16 };
	std::wstring map;
};
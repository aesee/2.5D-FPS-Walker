#pragma once
#include "EngineLib.h"
#include <iostream>

class Map
{
public:
	Map(std::string path);
	~Map();
	Vector2D GetSize() { return m_mapSize; }
	bool IsWallIn(int x, int y);

private:
	Vector2D m_mapSize = Vector2D{ 16, 16 };
	std::wstring m_map;
};
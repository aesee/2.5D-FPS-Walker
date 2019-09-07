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

Map::Map()
{
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
}

Map::~Map()
{

}

bool Map::IsWallIn(int x, int y)
{
	return map.c_str()[x * static_cast<int>(mapSize.x) + y] == '#';
}

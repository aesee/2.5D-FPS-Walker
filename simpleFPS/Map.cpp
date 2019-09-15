#include "stdafx.h"
#include "Map.h"

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

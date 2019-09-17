#include "Map.h"

Map::Map()
{
	// Create Map of world space # = wall block, . = space
	m_map.empty();
	m_map += L"################";
	m_map += L"#..............#";
	m_map += L"#.......########";
	m_map += L"#..............#";
	m_map += L"#......##......#";
	m_map += L"#......##......#";
	m_map += L"#..............#";
	m_map += L"###............#";
	m_map += L"##.............#";
	m_map += L"#......####..###";
	m_map += L"#......#.......#";
	m_map += L"#......#.......#";
	m_map += L"#..............#";
	m_map += L"#......#########";
	m_map += L"#..............#";
	m_map += L"################";
}

Map::~Map()
{

}

bool Map::IsWallIn(int x, int y)
{
	return m_map.c_str()[x * static_cast<int>(m_mapSize.x) + y] == '#';
}

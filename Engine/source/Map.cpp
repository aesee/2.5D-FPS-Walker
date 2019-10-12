#include "Map.h"
#include "Tools/FileManager.h"
#include <memory>

Map::Map(std::string path)
{
	// Load map from resources
	FileManager::ReadStringFromFile(path, m_map);
}

Map::~Map()
{

}

bool Map::IsWallIn(int x, int y)
{
	return m_map.c_str()[x * static_cast<int>(m_mapSize.x) + y] == '#';
}

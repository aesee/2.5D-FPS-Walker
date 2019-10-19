#include "Map.h"
#include "Tools/FileManager.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iterator>

Map::Map(std::string path)
{
	// Load map from resources
	/*std::wstring source;
	FileManager::ReadStringFromFile(path, source);

	if (source.length() < 1)
	{
		return;
	}*/

	auto ParsePlayerData = [this](std::string& line)
	{
		std::vector<std::string> parameters;
		std::istringstream parserStream(line);
		std::copy(std::istream_iterator<std::string>(parserStream),
			std::istream_iterator<std::string>(),
			std::back_inserter(parameters));

		m_playerStartPosition.x = std::stof(parameters[1]);
		m_playerStartPosition.y = std::stof(parameters[2]);
		m_playerStartPosition.z = 0;
		m_playerStartRotation = std::stof(parameters[3]);
	};

	auto ParseSectorData = [this](std::string& line, std::vector<Vector2>& vertex)
	{
		Sector sector;
		std::vector<int> num;

		std::vector<std::string> parameters;
		std::istringstream parserStream(line);
		std::copy(std::istream_iterator<std::string>(parserStream),
			std::istream_iterator<std::string>(),
			std::back_inserter(parameters));

		sector.floor = std::stoi(parameters[1]);
		sector.ceil = std::stoi(parameters[2]);

		for (int i = 3; i < parameters.size(); ++i)
		{
			num.push_back(std::stoi(parameters[i]));
		}
		int nSectorParameters = static_cast<int>(parameters.size() - 2);
		sector.npoints = nSectorParameters /= 2;
		sector.neighbors = new signed char[nSectorParameters];
		sector.vertex = new Vector2[nSectorParameters + 1];
		for (int i = 0; i < nSectorParameters; ++i)
		{
			sector.neighbors[i] = num[nSectorParameters + i];
		}
		for (int i = 0; i < nSectorParameters; ++i)
		{
			sector.vertex[i + 1] = vertex[num[i]];
		}
		sector.vertex[0] = sector.vertex[nSectorParameters]; // Ensure the vertexes form a loop
		sectors.push_back(sector);
	};

	auto ParseVertexData = [](std::string& line, std::vector<Vector2>& vertex)
	{
		std::vector<std::string> parameters;
		std::istringstream parserStream(line);
		std::copy(std::istream_iterator<std::string>(parserStream),
			std::istream_iterator<std::string>(),
			std::back_inserter(parameters));

		for (int i = 2; i < parameters.size(); ++i)
		{
			Vector2 vertice(std::stof(parameters[i]), std::stof(parameters[1]));
			vertex.push_back(vertice);
		}
	};

	std::vector<Vector2> vertex;

	std::ifstream infile(path);
	std::string line;
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		
		switch (line[0])
		{
		case 'v': // vertex
			ParseVertexData(line, vertex);
			break;
		case 's': // sector
			ParseSectorData(line, vertex);
			break;
		case 'p': // player
			ParsePlayerData(line);
			break;
		}
	}
}

Map::~Map()
{

}

bool Map::IsWallIn(int x, int y)
{
	//return m_map.c_str()[x * static_cast<int>(m_mapSize.x) + y] == '#';
	return false;
}

Vector3& Map::GetPlayerStartPosition()
{
	return m_playerStartPosition;
}

float Map::GetPlayerStartRotation()
{
	return m_playerStartRotation;
}

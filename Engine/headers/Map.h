#pragma once
#include "EngineLib.h"
#include <iostream>
#include <vector>

struct Sector
{
	int floor, ceil;
	Vector2 *vertex;				// Each vertex has an x and y coordinate
	signed char *neighbors;           // Each edge may have a corresponding neighboring sector
	unsigned npoints;                 // How many vertexes there are
};

struct NodeSector
{
	int sectorno;
	int	sx1;
	int	sx2;
};

class Map
{
public:
	Map(std::string path);
	~Map();
	//Vector2 GetSize() { return m_mapSize; }
	bool IsWallIn(int x, int y);

	Vector3& GetPlayerStartPosition();
	float GetPlayerStartRotation();

private:
	//Vector2 m_mapSize = Vector2{ 16, 16 };
	//std::wstring m_map;

	Vector3 m_playerStartPosition;
	float m_playerStartRotation;

public:
	std::vector<Sector> sectors;
};
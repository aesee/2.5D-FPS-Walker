#pragma once
#include <chrono>

class Input
{
public:
	void UpdatePlayerMove(class Map& map);

private:
	std::chrono::time_point<std::chrono::system_clock> tp1;
	std::chrono::time_point<std::chrono::system_clock> tp2;
};
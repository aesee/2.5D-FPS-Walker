#pragma once
#include "Player.h"

struct Controls
{
	char moveForward = 'W';
	char moveBackward = 'S';
	char turnRight = 'D';
	char turnLeft = 'A';
};

class GameConfig
{
public:
	static GameConfig& Get();
	Controls& GetControls();
	Player& GetCurrentPlayer();

private:
	GameConfig() {};

private:
	Player player;
	Controls controls;

public:
	float FOV = 1.5708f;			// Field of View
	float Depth = 30.0f;			// Maximum rendering distance
	float Speed = 1.5f;				// Walking Speed
};

#pragma once

class GameConfig
{
public:
	static GameConfig& Get();

private:
	GameConfig() {};

private:

public:
	float FOV = 1.5708f;			// Field of View
	float Depth = 30.0f;			// Maximum rendering distance
	float Speed = 1.5f;				// Walking Speed
};

#pragma once

class GameConfig
{
public:
	static GameConfig& Get();

private:
	GameConfig() {};

private:
	static GameConfig* m_gameConfig;

public:
	float FOV = 3.14159f / 3.0f;	// Field of View
	float Depth = 30.0f;			// Maximum rendering distance
	float Speed = 1.5f;			// Walking Speed
};

#include "GameConfig.h"

GameConfig& GameConfig::Get()
{
	static GameConfig* s_gameConfig = new GameConfig;
	return *s_gameConfig;
}

Controls& GameConfig::GetControls()
{
	return controls;
}

Player& GameConfig::GetCurrentPlayer()
{
	return player;
}

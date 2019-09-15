#include "stdafx.h"
#include "GameConfig.h"

GameConfig* GameConfig::gameConfig = nullptr;

GameConfig& GameConfig::Get()
{
	if (!gameConfig)
	{
		gameConfig = new GameConfig;
	}

	return *gameConfig;
}
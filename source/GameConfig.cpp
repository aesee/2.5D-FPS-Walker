#include "GameConfig.h"

GameConfig* GameConfig::m_gameConfig = nullptr;

GameConfig& GameConfig::Get()
{
	if (!m_gameConfig)
	{
		m_gameConfig = new GameConfig;
	}

	return *m_gameConfig;
}
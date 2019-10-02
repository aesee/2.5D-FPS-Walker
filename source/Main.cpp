#include "simpleFPS.h"
#include "WindowApp.h"
#include "Renderer.h"
#include "GameConfig.h"
#include "Map.h"
#include "Player.h"
#include "Input.h"

// Set main() as startup point for windows linker
#ifdef _MSC_VER
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
	WindowApp window;	
	GameConfig& gameConfig = GameConfig::Get();	
	Input input(window.GetInstance());
	
	// Init map
	Map map;
	std::unique_ptr<Renderer> renderer = window.CreateRenderer(gameConfig.FOV, gameConfig.Depth);
	renderer->SetMap(&map);

	// Create player
	Player player;

	// Game loop:
	while (true)
	{
		if (window.IsUserWantsToExit())
		{
			break;
		}

		// Input
		input.UpdateInput(gameConfig, player, map);

		// Render
		renderer->DrawFrame(player.GetPosition(), player.GetRotation());
	}

	return window.GetState();
}
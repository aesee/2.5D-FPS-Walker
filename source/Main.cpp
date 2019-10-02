#include <memory>
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
	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(*window.CreateRenderer(gameConfig.FOV, gameConfig.Depth));
	renderer->SetMap(&map);

	// Create player
	Vector2D StartLocation = Vector2D(14.7f, 5.09f);
	Player player(StartLocation);

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
		Vector2D PlayerPosition = player.GetPosition();
		float PlayerRotation = player.GetRotation();
		renderer->DrawFrame(PlayerPosition, PlayerRotation);
	}

	return window.GetState();
}
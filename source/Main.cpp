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
	Input input;

	// Init map
	Map map;
	Renderer renderer(window.GetDeviceContext(), gameConfig.FOV, gameConfig.Depth);
	renderer.SetScreenSize(window.GetScreenSize());
	renderer.SetMap(&map);

	// Create player
	Vector2D StartLocation = Vector2D(10.0f, 5.09f);
	//Player player(StartLocation);
	Player& player = gameConfig.GetCurrentPlayer();
	player.SetPosition(StartLocation);

	// Game loop:
	while (true)
	{
		if (window.IsUserWantsToExit())
		{
			break;
		}

		// Input
		input.UpdateInput(map); // TODO : remove map from input

		// Render
		Vector2D PlayerPosition = player.GetPosition();
		float PlayerRotation = player.GetRotation();
		renderer.DrawFrame(PlayerPosition, PlayerRotation);
	}

	return window.GetState();
}
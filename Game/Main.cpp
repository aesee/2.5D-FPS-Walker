#include "Game.h"
#include "Map.h"
#include "Player.h"

// Set main() as startup point for windows linker
#ifdef _MSC_VER
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
	Game& game = Game::Get();	
	
	// Init map
	Map testMap;
	game.SetNewMap(&testMap);

	// Create player
	Vector2D startLocation = Vector2D(10.0f, 5.09f);
	Player& player = game.GetCurrentPlayer();
	player.SetPosition(startLocation);

	// Game loop:
	while (game.Update())
	{ }

	return game.GetApplicationState();
}
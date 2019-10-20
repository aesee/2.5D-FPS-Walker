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
	Map testMap("../Levels/TestLevel2.lvl");
	game.SetNewMap(&testMap);

	// Game loop:
	while (game.Update())
	{ }

	return game.GetApplicationState();
}
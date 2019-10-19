#include "Input.h"
#include "Player.h"
#include "Game.h"
#include "Map.h"

void Input::UpdatePlayerMove(Map& map)
{
	tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	tp1 = tp2;
	float fElapsedTime = elapsedTime.count();

	Game& gameConfig = Game::Get();
	Player& player = gameConfig.GetCurrentPlayer();

	if (player.GetMoveStatus() != 0)
	{
		Vector3 playerPosition = player.GetPosition();
		float playerRotation = player.GetRotation();

		float movementSpeed = gameConfig.speed * fElapsedTime;
		Vector3 offsetPosition = Vector3(cosf(playerRotation) * movementSpeed, sinf(playerRotation) * movementSpeed, 0);
		Vector3 newPosition = playerPosition + offsetPosition * player.GetMoveStatus();

		if (!map.IsWallIn(static_cast<int>(round(newPosition.x)), static_cast<int>(round(newPosition.y))))
		{
			player.SetPosition(newPosition);
		}
	}

	if (player.GetTurnStatus() != 0)
	{
		float newRotation = player.GetRotation() + (gameConfig.speed * 0.75f) * player.GetTurnStatus() * fElapsedTime;
		player.SetRotation(newRotation);
	}
}
#include "Input.h"
#include "Player.h"
#include "Game.h"
#include "Resource.h"
#include "Map.h"

// Command pattern
namespace
{
	class IInputCommand
	{
	public:
		virtual ~IInputCommand() {}
		virtual void Execute(Player& player) = 0;
		virtual void Undo(Player& player) = 0;
	};

	class MoveForwardCommand : public IInputCommand
	{
	public:
		virtual ~MoveForwardCommand() override {};
		virtual void Execute(Player& player) override
		{
			player.SetMoveStatus(1);
		}
		virtual void Undo(Player& player) override
		{
			player.SetMoveStatus(0);
		}
	};

	class MoveBackwardCommand : public IInputCommand
	{
	public:
		virtual ~MoveBackwardCommand() override {};
		virtual void Execute(Player& player) override
		{
			player.SetMoveStatus(-1);
		}
		virtual void Undo(Player& player) override
		{
			player.SetMoveStatus(0);
		}
	};

	class TurnRightCommand : public IInputCommand
	{
	public:
		virtual ~TurnRightCommand() override {};
		virtual void Execute(Player& player) override
		{
			player.SetTurnStatus(1);
		}
		virtual void Undo(Player& player) override
		{
			player.SetTurnStatus(0);
		}
	};

	class TurnLeftCommand : public IInputCommand
	{
	public:
		virtual ~TurnLeftCommand() override {};
		virtual void Execute(Player& player) override
		{
			player.SetTurnStatus(-1);
		}
		virtual void Undo(Player& player) override
		{
			player.SetTurnStatus(0);
		}
	};
}

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
		Vector2D playerPosition = player.GetPosition();
		float playerRotation = player.GetRotation();

		float movementSpeed = gameConfig.speed * fElapsedTime;
		Vector2D offsetPosition = Vector2D(sinf(playerRotation) * movementSpeed, cosf(playerRotation) * movementSpeed);
		Vector2D newPosition = playerPosition + offsetPosition * player.GetMoveStatus();

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

LRESULT CALLBACK Input::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	if (message == WM_KEYDOWN)
	{
		Game& gameConfig = Game::Get();
		Controls& controls = gameConfig.GetControls();

		if (wParam == controls.turnLeft)
		{
			TurnLeftCommand Command;
			Command.Execute(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.turnRight)
		{
			TurnRightCommand Command;
			Command.Execute(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.moveForward)
		{
			MoveForwardCommand Command;
			Command.Execute(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.moveBackward)
		{
			MoveBackwardCommand Command;
			Command.Execute(gameConfig.GetCurrentPlayer());
		}

		return 0;
	}

	if (message == WM_KEYUP)
	{
		Game& gameConfig = Game::Get();
		Controls& controls = gameConfig.GetControls();

		if (wParam == controls.turnLeft)
		{
			TurnLeftCommand Command;
			Command.Undo(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.turnRight)
		{
			TurnRightCommand Command;
			Command.Undo(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.moveForward)
		{
			MoveForwardCommand Command;
			Command.Undo(gameConfig.GetCurrentPlayer());
		}

		if (wParam == controls.moveBackward)
		{
			MoveBackwardCommand Command;
			Command.Undo(gameConfig.GetCurrentPlayer());
		}

		return 0;
	}

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}

	return 0;
}
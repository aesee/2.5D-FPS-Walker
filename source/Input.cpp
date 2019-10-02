#include "Input.h"
#include "Player.h"
#include "GameConfig.h"
#include "Map.h"
#include "Resource.h"

Input::InputKeys Input::inputKeys;

Input::Input(HINSTANCE* inHInst)
	:hInst(inHInst)
{}

void Input::UpdateInput(GameConfig& gameConfig, Player& player, Map& map)
{
	tp2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = tp2 - tp1;
	tp1 = tp2;
	float fElapsedTime = elapsedTime.count();

	if (inputKeys.A)
	{
		float newRotation = player.GetRotation() - (gameConfig.Speed * 0.75f) * fElapsedTime;
		player.SetRotation(newRotation);
	}

	if (inputKeys.D)
	{
		float newRotation = player.GetRotation() + (gameConfig.Speed * 0.75f) * fElapsedTime;
		player.SetRotation(newRotation);
	}

	if (inputKeys.W)
	{
		Vector2D playerPosition = player.GetPosition();
		float playerRotation = player.GetRotation();

		float movementSpeed = gameConfig.Speed * fElapsedTime;
		Vector2D offsetPosition = Vector2D(sinf(playerRotation) * movementSpeed, cosf(playerRotation) * movementSpeed);
		player.SetPosition(playerPosition + offsetPosition);

		if (map.IsWallIn((int)playerPosition.x, (int)playerPosition.y))
		{
			player.SetPosition(playerPosition);
		}
	}

	if (inputKeys.S)
	{
		Vector2D playerPosition = player.GetPosition();
		float playerRotation = player.GetRotation();

		float movementSpeed = gameConfig.Speed * fElapsedTime;
		Vector2D offsetPosition = Vector2D(sinf(playerRotation) * movementSpeed, cosf(playerRotation) * movementSpeed);
		player.SetPosition(playerPosition - offsetPosition);

		if (map.IsWallIn((int)playerPosition.x, (int)playerPosition.y))
		{
			player.SetPosition(playerPosition);
		}
	}
}

LRESULT CALLBACK Input::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case 'A':
		{
			Input::inputKeys.A = true;
			break;
		}
		case 'D':
		{
			Input::inputKeys.D = true;
			break;
		}
		case 'W':
		{
			Input::inputKeys.W = true;
			break;
		}
		case 'S':
		{
			Input::inputKeys.S = true;
			break;
		}
		}

		return 0;
	}

	if (message == WM_KEYUP || message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case 'A':
		{
			Input::inputKeys.A = false;
			break;
		}
		case 'D':
		{
			Input::inputKeys.D = false;
			break;
		}
		case 'W':
		{
			Input::inputKeys.W = false;
			break;
		}
		case 'S':
		{
			Input::inputKeys.S = false;
			break;
		}
		}

		return 0;
	}

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
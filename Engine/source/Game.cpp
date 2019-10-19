#include "Game.h"
#include "Player.h"
#include "Input/Input.h"
#include "Windows/WindowApp.h"
#include "Renderer/Renderer.h"
#include "Map.h"

Game& Game::Get()
{
	static Game* s_gameConfig = new Game();
	return *s_gameConfig;
}

Controls& Game::GetControls()
{
	return *m_controls;
}

Player& Game::GetCurrentPlayer()
{
	return *m_player;
}

bool Game::Update()
{
	if (m_window->IsUserWantsToExit())
	{
		return false;
	}

	// Input
	m_input->UpdatePlayerMove(*m_map); // TODO : remove map from input

	// Render
	Vector3 playerPosition = m_player->GetPosition();
	float playerRotation = m_player->GetRotation();
	m_renderer->DrawFrame(playerPosition, playerRotation);

	return true;
}

int Game::GetApplicationState()
{
	return m_window->GetState();
}

void Game::SetNewMap(Map* map)
{
	m_map = map;
	m_renderer->SetMap(map);

	m_player->SetPosition(m_map->GetPlayerStartPosition());
	m_player->SetRotation(m_map->GetPlayerStartRotation());
}

Game::Game()
{
	m_controls = new Controls();
	m_window = new WindowApp();
	m_player = new Player();
	m_input = new Input;

	IntVector2D screenResolution = m_window->GetScreenSize();
	m_renderer = new Renderer(m_window->GetDeviceContext(), Vector2(0.73f * screenResolution.y, 0.2f * screenResolution.y), m_depth);
	m_renderer->SetScreenSize(screenResolution);
}

Game::~Game()
{
	delete m_player;
	delete m_controls;
	delete m_input;
	delete m_renderer;
	delete m_window;
}

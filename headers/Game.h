#pragma once

struct Controls
{
	char moveForward = 'W';
	char moveBackward = 'S';
	char turnRight = 'D';
	char turnLeft = 'A';
};

class Player;
class Map;
class Input;
class WindowApp;
class Renderer;

class Game
{
public:
	static Game& Get();
	Controls& GetControls();
	Player& GetCurrentPlayer();
	bool Update();
	int GetApplicationState();
	void SetNewMap(Map* map);

private:
	Game();
	~Game();

private:
	WindowApp* m_window;
	Player* m_player;
	Controls* m_controls;
	Input* m_input;
	Renderer* m_renderer;

	Map* m_map;

private:
	float m_FOV = 1.5708f;			// Field of View
	float m_depth = 30.0f;			// Maximum rendering distance
public:
	float speed = 1.5f;				// Walking Speed
};

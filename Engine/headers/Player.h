#pragma once
#include "EngineLib.h"

class Player
{
public:
	Player(Vector2D InitialPosition = Vector2D(0, 0), float InitialRotation = 0);
	~Player() {}

	void SetPosition(Vector2D newPosition);
	void SetRotation(float newRotation);
	Vector2D GetPosition();
	float GetRotation() { return m_rotation; }

	int GetMoveStatus();
	void SetMoveStatus(int status);
	int GetTurnStatus();
	void SetTurnStatus(int status);


private:
	Vector2D m_position;
	float m_rotation;			// Player Start Rotation

	int m_moveStatus = 0;
	int m_turnStatus = 0;
};
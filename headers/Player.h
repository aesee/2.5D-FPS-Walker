#pragma once
#include "EngineLib.h"

class Player
{
public:
	Player() {};
	~Player() {}

	void SetPosition(Vector2D newPosition) { m_position = newPosition; }
	void SetRotation(float newRotation) { m_rotation = newRotation; }
	Vector2D GetPosition() { return m_position; }
	float GetRotation() { return m_rotation; }

private:
	Vector2D m_position = Vector2D{ 14.7f, 5.09f };
	float m_rotation = 0.0f;			// Player Start Rotation
};
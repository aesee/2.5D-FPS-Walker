#pragma once
#include "EngineLib.h"

class Player
{
public:
	Player() {};
	~Player() {}

	void SetPosition(Vector2D newPosition) { position = newPosition; }
	void SetRotation(float newRotation) { rotation = newRotation; }
	Vector2D GetPosition() { return position; }
	float GetRotation() { return rotation; }

private:
	Vector2D position = Vector2D{ 14.7f, 5.09f };
	float rotation = 0.0f;;			// Player Start Rotation
};
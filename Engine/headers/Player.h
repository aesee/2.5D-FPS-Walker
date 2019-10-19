#pragma once
#include "EngineLib.h"

class Player
{
public:
	Player(Vector3 InitialPosition = Vector3(), float InitialRotation = 0);
	~Player() {}

	void SetPosition(Vector3 newPosition);
	void SetRotation(float newRotation);
	Vector3 GetPosition();
	float GetRotation() { return m_rotation; }

	int GetMoveStatus();
	void SetMoveStatus(int status);
	int GetTurnStatus();
	void SetTurnStatus(int status);

private:
	Vector3 m_position;
	float m_rotation;			// Player Start Rotation

	int m_moveStatus = 0;
	int m_turnStatus = 0;
};
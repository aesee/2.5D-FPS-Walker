#include "Player.h"


Player::Player(Vector2D InitialPosition, float InitialRotation)
	: m_position(InitialPosition), m_rotation(InitialRotation)
{

}

void Player::SetPosition(Vector2D newPosition)
{
	m_position = newPosition;
}

void Player::SetRotation(float newRotation)
{
	m_rotation = newRotation;
}

Vector2D Player::GetPosition()
{
	return m_position;
}

int Player::GetMoveStatus()
{
	return m_moveStatus;
}

void Player::SetMoveStatus(int status)
{
	if (status > 1)
	{
		status = 1;
	}

	if (status < -1)
	{
		status = -1;
	}

	m_moveStatus = status;
}

int Player::GetTurnStatus()
{
	return m_turnStatus;
}

void Player::SetTurnStatus(int status)
{
	if (status > 1)
	{
		status = 1;
	}

	if (status < -1)
	{
		status = -1;
	}

	m_turnStatus = status;
}

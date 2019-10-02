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

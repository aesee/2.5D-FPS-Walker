#include "EngineLib.h"

Vector2D Vector2D::operator+(Vector2D vector)
{
	return Vector2D(x + vector.x, y + vector.y);
}

Vector2D Vector2D::operator-(Vector2D vector)
{
	return Vector2D(x - vector.x, y - vector.y);
}

Vector2D Vector2D::operator*(int value)
{
	return Vector2D(x * value, y * value);
}

IntVector2D IntVector2D::operator+(IntVector2D vector)
{
	return IntVector2D(x + vector.x, y + vector.y);
}

IntVector2D IntVector2D::operator-(IntVector2D vector)
{
	return IntVector2D(x - vector.x, y - vector.y);
}

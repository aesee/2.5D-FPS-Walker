#include "EngineLib.h"

Vector2 Vector2::operator+(Vector2 vector)
{
	return Vector2(x + vector.x, y + vector.y);
}

Vector2 Vector2::operator-(Vector2 vector)
{
	return Vector2(x - vector.x, y - vector.y);
}

Vector2 Vector2::operator*(int value)
{
	return Vector2(x * value, y * value);
}

float Vector2::CrossProduct(Vector2& Vector)
{
	return (x * Vector.y - Vector.x * y);
}

IntVector2D IntVector2D::operator+(IntVector2D vector)
{
	return IntVector2D(x + vector.x, y + vector.y);
}

IntVector2D IntVector2D::operator-(IntVector2D vector)
{
	return IntVector2D(x - vector.x, y - vector.y);
}

Vector3::Vector3()
	: x(0), y(0), z(0)
{ }

Vector3::Vector3(float inX, float inY, float inZ)
	: x(inX), y(inY), z(inZ)
{ }

Vector3 Vector3::operator+(Vector3 vector)
{
	return Vector3(x + vector.x, y + vector.y, z + vector.z);
}

Vector3 Vector3::operator*(int value)
{
	return Vector3(x * value, y * value, z * value);
}

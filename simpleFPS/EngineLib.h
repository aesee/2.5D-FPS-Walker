#pragma once

struct Vector2D
{
	Vector2D(float inX, float inY)
		: x(inX), y(inY) {}

	float x;
	float y;

	Vector2D operator +(Vector2D vector)
	{
		return Vector2D(x + vector.x, y + vector.y);
	}

	Vector2D operator -(Vector2D vector)
	{
		return Vector2D(x - vector.x, y - vector.y);
	}
};
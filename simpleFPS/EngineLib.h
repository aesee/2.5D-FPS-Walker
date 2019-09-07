#pragma once

struct Vector2D
{
	Vector2D(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

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
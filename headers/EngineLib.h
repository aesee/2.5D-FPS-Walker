#pragma once
#include "stdint.h"

struct Vector2D
{
	Vector2D(float inX, float inY)
		: x(inX), y(inY) {}

	float x;
	float y;

	Vector2D operator +(Vector2D vector);
	Vector2D operator -(Vector2D vector);
	Vector2D operator *(int value);
};

struct IntVector2D
{
	IntVector2D()
		:x(0), y(0){}

	IntVector2D(int32_t inX, int32_t inY)
		:x(inX), y(inY) {}

	int32_t x;
	int32_t y;

	IntVector2D operator +(IntVector2D vector);
	IntVector2D operator -(IntVector2D vector);
};
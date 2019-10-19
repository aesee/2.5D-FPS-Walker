#pragma once
#include "stdint.h"

struct Vector2
{
	Vector2()
		: x(0), y(0) {}

	Vector2(float inX, float inY)
		: x(inX), y(inY) {}

	float x;
	float y;

	float CrossProduct(Vector2& Vector);

	Vector2 operator +(Vector2 vector);
	Vector2 operator -(Vector2 vector);
	Vector2 operator *(int value);
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

struct Vector3
{
	Vector3();
	Vector3(float inX, float inY, float inZ);

	float x;
	float y;
	float z;

	Vector3 operator *(int value);
	Vector3 operator +(Vector3 vector);
};
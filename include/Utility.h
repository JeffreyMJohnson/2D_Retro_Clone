#pragma once
#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <math.h>
#define PI 3.14159

enum TYPE
{
	PLAYER,
	ENEMY
};


struct Point2d
{
	float x, y;

	Point2d()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Point2d(float a_x, float a_y)
	{
		x = a_x;
		y = a_y;
	}

	inline bool operator==(const Point2d& other)
	{
		return (x == other.x && y == other.y);

	}

	inline bool operator!=(const Point2d& other)
	{
		return !(*this == other);
	}
};

class Helper
{
public:
	/*
	returns the given angle in degrees converted to radians
	*/
	static float DegreeToRadians(float angleInDegrees)
	{
		return angleInDegrees * (PI / 180);
	}

	/*
	returns given angle in radians converted to degrees
	*/
	static float RadiansToDegrees(float angleInRadians)
	{
		return angleInRadians * (180 / PI);
	}

	/*
	returns true if given result is within delta of expected result using 
	formula: (fabs(result - expected) < delta)
	*/
	static bool FloatEquals(const float& result, const float& expected, const float delta)
	{
		return (fabs(result - expected) < delta);
	}
};
#endif
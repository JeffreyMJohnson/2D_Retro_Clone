#pragma once
#ifndef _UTILITY_H_
#define _UTILITY_H_

#define PI 3.14159

enum TYPE
{
	PLAYER,
	ENEMY
};


struct Point2d
{
	float x, y;
};

class Helper
{
public:
	static float DegreeToRadians(float angleInDegrees)
	{
		return angleInDegrees * (PI / 180);
	}

	static float RadiansToDegrees(float angleInRadians)
	{
		return angleInRadians * (180 / PI);
	}
};
#endif
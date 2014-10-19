#pragma once


#ifndef _BULLET_H_
#define _BULLET_H_

#include "AIE.h"
#include "Entity.h"
extern const int screenHeight;

class Bullet
{
public:


	Bullet();
	~Bullet();

	float x;
	float y;

	float width;
	float height;

	unsigned int textureId;

	//float velocityX;
	float velocityY;

	void Update(float a_delta);
	void Draw();

	bool isCollided(Entity* enemy);

	void InitializeBullet(float a_x, float a_y, float a_velocityX, float a_velocity_y, unsigned int a_textureId);

	bool isActive;
};
#endif // !_BULLET_H_



#pragma once


#ifndef _BULLET_H_
#define _BULLET_H_

#include "AIE.h"
#include "Entity.h"
extern const int screenHeight;

class Bullet : public Entity
{
public:
	int health;
	bool alive;

	Bullet(const char* filePath, float a_width, float a_height);
	
	void Init(Point2d a_pos, Point2d a_velocity, float a_speed, float a_radius, int health);
	
	//move this bullet to given position with velocity, speed and health
	void Spawn(Point2d a_pos, Point2d a_velocity, float a_speed, int a_health);

	void Update(float a_delta);
	void Draw();

	~Bullet();
};
#endif // !_BULLET_H_



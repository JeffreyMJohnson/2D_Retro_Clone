#pragma once

#ifndef _PLAYER_H_
#define  _PLAYER_H_

#include "Entity.h"
#include "Utility.h"
#include "BulletManager.h"

class Player : public Entity
{
public:
	unsigned int spriteID_A;
	unsigned int spriteId_B;
	
	//Instantiate player with sprite
	Player(char* filePath, float width, float height);

	//initialize player with position, velocity, radius (collider),health, speed and alive
	void Init(Point2d a_pos, Point2d a_velocity, float a_radius, int a_health);

	//input handling
	void Input();

	//handle shooting
	void Shoot();

	//Must implement these methods before can instantiate class
	void Update(float a_delta);
	void Draw();

	~Player();

private:
	int health;
	

};


#endif // _PLAYER_H_
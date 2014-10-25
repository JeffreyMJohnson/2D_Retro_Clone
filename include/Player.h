#pragma once

#ifndef _PLAYER_H_
#define  _PLAYER_H_

#include "Entity.h"
#include "Utility.h"
#include "BulletManager.h"
#include <iostream>

struct Score
{
	int value;
	std::string value_s;

	char* ToString()
	{
		char buff[6];
		sprintf(buff, "%010d", value);
		return buff;
	}
};

const int MAX_BULLETS = 20;

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

	//convenience functions
	void SetMovementKeys(unsigned int a_moveLeft, unsigned int a_moveRight);
	void SetMovementExtremes(unsigned int a_leftExtreme, unsigned int a_rightExtreme);

	//Must implement these methods before can instantiate class
	virtual void Update(float a_delta);
	virtual void Draw();


	
	//standard setters/getters

	void SetMoveLeftKey(unsigned int a_moveKey);
	unsigned int GetLeftMoveKey();

	void SetMoveRightgKey(unsigned int a_moveKey);
	unsigned int GetMoveRightKey();

	void SetShootKey(unsigned int a_shootKey);
	unsigned int GetShootKey();

	void SetLeftMovementExtreme(unsigned int a_movementExtreme);
	unsigned int GetLeftMovementExtreme();

	void SetRightMovementExtreme(unsigned int a_movementExtreme);
	unsigned int GetRightMovementExtreme();
	
	//int GetScore();
	//char* GetScoreAsString();

	~Player();

	unsigned int shootKey;

private:
	//float speed;
	int health;
	unsigned int moveLeftKey;
	unsigned int moveRightKey;
	
	unsigned int leftMovementExtreme;
	unsigned int rightMovementExtreme;


	//offset for bullet position in relation to player
	float bulletYOffset;
	/*float currentReloadBulletTime;
	float maxBulletReloadTime;*/
	

};


#endif // _PLAYER_H_
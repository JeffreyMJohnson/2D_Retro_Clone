#pragma once


#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "AIE.h"
#include "Entity.h"

extern const int screenWidth;
extern const int screenHeight;
extern const int NUM_ENEMYS;

struct Point2d
{
	float x, y;
};

class Enemy : public Entity
{
public:

	static int activeEnemyCount;
	bool isAttacking;

	Enemy();

	virtual void Update(float delta);
	virtual void Draw();

	void SetSpeed(float a_speed);
	float GetSpeed();

	//helper functions for convenience 
	void setMovementExtremes(unsigned int a_leftExtreme, unsigned int a_rightExtreme);
	
	////move enemy 
	//void Move(float a_speed, int a_direction, float a_delta);

	void Attack(float timeDelta);

	void SetSpeedX(float a_speedX);
	float GetSpeedX();

	void SetSpeedY(float a_speedY);
	float GetSpeedY();

	void SetLeftMoveExtreme(unsigned int a_leftExtreme);
	unsigned int GetLeftMoveExtreme();

	void SetRightMoveExtreme(unsigned int a_leftExtreme);
	unsigned int GetRightMoveExtreme();
	
	void SetScoreValue(int a_scoreValue);
	int GetScoreValue();

	void SetDirection(int a_direction);
	int GetDirection();

	void SetIsActive(bool a_isActive);
	bool GetIsActive();

	void SetOriginalPos(Point2d a_point);
	Point2d GetOriginalPosition();

	void SetAttackAngle(float a_angle);
	float GetAttackAngle();

	void SetAttackRadius(float a_radius);
	float GetAttackRadius();

	

	~Enemy();	

	

private:
	unsigned int leftMovementExtreme;
	unsigned int rightMovementExtreme;
	int scoreValue;
	bool isActive;
	float speed;
	int direction;
	Point2d originalPosition;
	float attackAngle;
	float attackRadius;
	
	
	

	/*
	check all collisions and apply effects:
	IsCollidedLeftWall - flip speedX and add a speed to speedY else speedY = 0
	IsCollideRighttWall - flip speedX and add a speed to speedY else speedY = 0
	params:
	a_speedY: Speed to set speedY variable to make enemy move along Y axis. Should
	be negative so enemy moves 'down'.
	*/
	void CheckCollisions();

	/*
	check if collided with right extreme (wall) and return true, otherwise return false
	*/
	bool IsCollidedRightWall();

	/*
	check if collided with left extreme (wall) and return true, otherwise return false
	*/
	bool IsCollidedLeftWall();
};


#endif // !_ENEMY_H_



#pragma once


#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "AIE.h"
//
#include "Entity.h"
#include "Player.h"
#include <vector>
#include <math.h>
#include <assert.h>

//#define PI 3.14159


extern const int screenWidth;
extern const int screenHeight;
extern const int NUM_ENEMYS;



enum attackStates
{
	WAIT,
	MOVE,
	CIRCLE,
	ATTACK,
	RETURN
};

class Enemy : public Entity
{
public:
	int health;
	bool alive;

	static int activeEnemyCount;
	bool isAttacking;
	bool isLeader;
	bool attackExitChosen;
	float attackSpeed;

	Player* player;
	//std::vector<Bullet*>* enemyBullets;

	//std::vector<Bullet*> enemyBullets;

	//these need to be hidden and have an easy way to increment the line
	float attackSlope;
	float attackYIntercept;
	//int attackDirection;
	Point2d attackVelocity;

	Enemy();

	//Instantiate enemy with sprite
	Enemy(const char* filePath, float a_width, float a_height);

	//Initialize enemy with position, velocity, collider radius, health, speed and alive
	void Init(Point2d a_pos, Point2d a_velocity, float a_radius, int a_health, float a_speed);

	//handle shooting
	void Shoot();


	virtual void Update(float delta);
	virtual void Draw();

	//float DegreeToRadians(float angleInDegrees);
	//float RadiansToDegrees(float angleInRadians);

	//void SetEnemyBullets(std::vector<Bullet*>* a_enemyBullets);

	//void SetSpeed(float a_speed);
	//float GetSpeed();

	//helper functions for convenience 
	void setMovementExtremes(unsigned int a_leftExtreme, unsigned int a_rightExtreme);

	////move enemy 
	//void Move(float a_speed, int a_direction, float a_delta);

	void Attack(float timeDelta);

	//void LoadBullets();

	//void SetSpeedX(float a_speedX);
	//float GetSpeedX();

	//void SetSpeedY(float a_speedY);
	//float GetSpeedY();

	void SetLeftMoveExtreme(unsigned int a_leftExtreme);
	unsigned int GetLeftMoveExtreme();

	void SetRightMoveExtreme(unsigned int a_leftExtreme);
	unsigned int GetRightMoveExtreme();

	void SetScoreValue(int a_scoreValue);
	int GetScoreValue();

	//void SetDirection(int a_direction);
	//int GetDirection();

	//void SetIsActive(bool a_isActive);
	//bool GetIsActive();

	void SetOriginalPos(Point2d a_point);
	Point2d GetreturnPosition();

	void SetAttackAngle(float a_angle);
	float GetAttackAngle();

	void SetAttackRadius(float a_radius);
	float GetAttackRadius();

	void SetAttackExitPoint(Point2d a_point);
	Point2d GetAttackExitPoint();

	void SetAttackState(attackStates a_state);
	attackStates GetAttackState();

	void SetReturnPosition(Point2d point);
	Point2d GetReturnPosition();

	~Enemy();



private:
	unsigned int leftMovementExtreme;
	unsigned int rightMovementExtreme;
	int scoreValue;
	//bool isActive;
	//float speed;
	//int direction;
	Point2d returnPosition;
	float attackAngle;
	float attackRadius;
	Point2d attackExitPoint;
	attackStates attackState;
	float shootTimer;
	float shootMaxTime;



	float GetSlopeOfLine(Point2d point1, Point2d point2);

	//Bullet* GetInactiveBullet();



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



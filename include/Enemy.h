#pragma once


#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "AIE.h"
//
#include "Entity.h"
#include "Player.h"
#include "GameState.h"
#include <vector>
#include <math.h>
#include <assert.h>


class GameState;

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
	bool attackExitChosen;
	float attackSpeed;

	Player* player;

	//these need to be hidden and have an easy way to increment the line
	float attackSlope;
	float attackYIntercept;
	Point2d attackVelocity;

	int colPositionIndex;
	int rowPositionIndex;

	Enemy();

	//Instantiate enemy with sprite
	Enemy(const char* filePath, float a_width, float a_height);

	/*
	returns true if this position is equal to the other position else returns false
	*/
	bool operator==(Enemy& other);

	/*
	returns true if this position is not equal to the other position else returns false
	*/
	bool operator!=(Enemy& other);

	//Initialize enemy with position, velocity, collider radius, health, speed, colIndex, rowIndex and alive
	void Init(Point2d a_pos, Point2d a_velocity, float a_radius, int a_health, float a_speed, int a_colIndex, int a_rowIndex);

	//handle shooting
	void Shoot();


	void Update(float delta);

	//overridden to give GameState handle for accessing position grid
	void Update(float delta, GameState* gameState);

	void Draw();

	//helper functions for convenience 
	void setMovementExtremes(unsigned int a_leftExtreme, unsigned int a_rightExtreme);
	
	void Attack(float timeDelta, GameState* gameState);

	void SetLeftMoveExtreme(unsigned int a_leftExtreme);
	unsigned int GetLeftMoveExtreme();

	void SetRightMoveExtreme(unsigned int a_leftExtreme);
	unsigned int GetRightMoveExtreme();

	void SetScoreValue(int a_scoreValue);
	int GetScoreValue();

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

	~Enemy();



private:
	unsigned int leftMovementExtreme;
	unsigned int rightMovementExtreme;
	int scoreValue;
	float attackAngle;
	float attackRadius;
	Point2d attackExitPoint;
	attackStates attackState;
	float shootTimer;
	float shootMaxTime;



	float GetSlopeOfLine(Point2d point1, Point2d point2);


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



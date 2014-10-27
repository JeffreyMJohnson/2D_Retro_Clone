#pragma once
#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "BaseState.h"
#include "BulletManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "EndGameState.h"
#include "HighScores.h"
#include <vector>
#include <string>
#include <time.h>
#include <assert.h>
#include <iostream>

//needed to prevent circular reference
class Enemy;

class GameState : public BaseState
{
public:
	GameState();
	~GameState();

	/*
	Called by machine state once on starting this state
	*/
	void Initialize();

	/*
	called by machine state each frame
	*/
	void Update(float a_timeStep, StateMachine* a_SMPointer);

	/*
	called by machine state each frame
	*/
	void Draw();

	/*
	this is called by state machine when this state is removed from stack, cleans up objects on
	the heap instead of class destructor
	*/
	void Destroy();

	Player* player;
	float restartTimer;
	float currentRestartTime;
	Point2d enemyGroupVelocity;
	float enemyGroupSpeed;
	bool gameOver;

	std::vector<float> enemyColPositions;
	std::vector<float> enemyRowPositions;
	
	Point2d GetEnemyGroupPosition(int colIndex, int rowIndex);


private:
	const float attackTimeMax = 2.0f;

	/*
	Creates each enemy object, setting their position in the group, and loading them in game objects array
	*/
	void CreateEnemies();

	/*
	enemy group moving logic, enemy bullet collision
	*/
	void EnemyLogic(Enemy* enemy, float timeDelta);

	/*
	bullet player collision logic, and enemy player collision logic
	*/
	void PlayerLogic(Player* a_player, float a_delta);

	/*
	Handle player after collision with enemy or enemy bullet.
	will switch state to game over if was last life otherwise will decrement lives remaining
	*/
	void PlayerDeath(Player* player);

	/*
	Helper function for attacking enemies
	*/
	void ChooseAttackers();

	/*
	Helper function for attacking enemies
	this will set all enemies in array to attack state of WAIT unless it's the first enemy
	constraint: attackingEnemies vector must have size > 0.
	This function runs once per attack cycle;
	*/
	void SetAttackLeader();

	/*
	returns true if the last alive enemy in attacking enemy list has returned to it's original position
	or there are no alive attacking enemies	else returns false
	*/
	bool lastEnemyReturned();

	/*Helper function for attacking enemies
	will check if the currently attacking enemy has moved to ATTACK state and if it is will change attack state
	of next enemy in list to MOVE if there is one.
	This function ran every frame*/
	void sendNextEnemy();

	/*
	helper function for attacking enemies
	*/
	int GetRandomDirection();

	/*
	helper function for attacking enemies
	returns max and min y position values for all enemies
	*/
	void GetEnemyColX(float& minX, float& maxX);

	/*
	invert the velocity on every alive enemy
	*/
	void ReverseEnemies();

	/*
	Helper file for attacking player
	*/
	void TimerTick(float timeDelta);

	/*
	Helper function for attacking player
	*/
	void GetColExtremes(float& minX, float& maxX);

	void GetAttackDirection();

	void DrawUI();

	/*
	initialize objects for new level. called when all enemy dead
	*/
	void NewLevelInit();

	char* ScoreToString(int a_score);


	bool sendAttack;
	Enemy* attackingEnemy;
	float attackTimer;
	float enemyColMinX;
	float enemyColMaxX;
	Point2d attackVelocity;

	std::vector<Entity*> gameObjects;
	std::vector<Enemy*> attackingEnemies;


	const char* scoreLabel;
	char scoreAsString[20];

	const char* highScoreLabel;
	char HighScoreAsString[20];


	//position of score and label
	Point2d scorePos;
	//position  of highscore and label
	Point2d highScorePos;

	int highScore;

	int playerLives;
	unsigned int playerLifeTextureID;
};
#endif // !_GAME_STATE_H_



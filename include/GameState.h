#pragma once
#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include "BaseState.h"
#include "AIE.h"
#include "StateMachine.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include <vector>
#include <string>
#include <time.h>
#include <assert.h>




class GameState : public BaseState
{
public:
	GameState();
	~GameState();

	void Initialize();
	void Update(float a_timeStep, StateMachine* a_SMPointer);
	void Draw();
	void Destroy();

	std::vector<Bullet*> enemyBullets;

private:
	const float attackTimeMax = 2.0f;


	void CreateEnemies();
	void MoveEnemies(float a_speed, int a_direction, float a_delta);
	void DrawEnemies();

	//void PlayerLogic(Player* a_player, float a_delta);
	void EnemyLogic(Enemy* enemy, float timeDelta);
	void ReverseEnemies();

	void DrawUI();

	bool sendAttack;

	void TimerTick(float timeDelta);

	void GetColExtremes(float& minX, float& maxX);

	void GetAttackDirection();

	void ChooseAttackers();

	void SetAttackLeader();

	int GetRandomDirection();

	//returns max and min y position values for all enemies
	void GetEnemyColX(float& minX, float& maxX);


	bool CheckCollision(float x1, float y1, float x2, float y2, float distance);

	char* ScoreToString(int a_score);


	float attackTimer;
	float enemyColMinX;
	float enemyColMaxX;
	int attackDirection;


	std::vector<Entity*> gameObjects;
	std::vector<Enemy*> attackingEnemies;

	//Bullet* bullet;
	//offset for bullet position in relation to player
	float bulletYOffset;


	std::string fontFile;

	const char* scoreLabel;
	char scoreAsString[20];

	const char* highScoreLabel;
	char HighScoreAsString[20];


	//position of score and label
	Point2d scorePos;
	//position  of highscore and label
	Point2d highScorePos;

	int score;
	int highScore;

	int playerLives;
	unsigned int playerLifeTextureID;

	//float attackAngle = 90.0f;

	/*
	char p1Score_s[20];
	char highScore_s[20];

	bool gameOver;
	float gameOverTimer;*/
};
#endif // !_GAME_STATE_H_



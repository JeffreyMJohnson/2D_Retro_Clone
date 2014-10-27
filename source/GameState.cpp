#include "GameState.h"

//global variables for position calculating
extern const int screenWidth;
extern const int screenHeight;

//Row/Col magic numbers
extern const int NUM_ENEMY_COLS;
extern const int NUM_ENEMY_ROWS;

//static score
int BaseState::score;

//PUBLIC FUNCTIONS
GameState::GameState()
{
	playerLives = 3;

	gameOver = false;
	scoreLabel = "1UP";
	highScoreLabel = "HIGH SCORE";
	scorePos = Point2d(screenWidth * .1f, screenHeight);
	highScorePos = Point2d(screenWidth * .5f - 75.0f, screenHeight);
	BaseState::score = 0;

	playerLifeTextureID = CreateSprite("./images/misc/user_life_sprite.png", 35.0f, 41.0f, true);
	srand(time(nullptr));
	sendAttack = false;
	enemyColMinX = 0.0f;
	enemyColMaxX = 0.0f;

	player = nullptr;

	//need to be a positive y for first stage of attack
	attackVelocity = Point2d(-1, 1);
	attackingEnemy = nullptr;

	restartTimer = 5.0f;
	currentRestartTime = 0.0f;
	enemyGroupVelocity = Point2d(1, 0);


	enemyGroupSpeed = 20.0f;

}

GameState::~GameState()
{
	//taken care of with Destroy function called by state machine.
}

/*
Called by machine state once on starting this state
*/
void GameState::Initialize()
{
	//score is static so make sure it's 0
	BaseState::score = 0;

	//initialize bulletManager static class
	BulletManager::Init();

	player = new Player("./images/player/galaxian_a.png", 46.0f, 69.0f);
	player->Init(Point2d(screenWidth * 0.5f, 100.0f), Point2d(), 25.0f, 1);

	gameObjects.push_back(player);

	//create and position enemy group
	CreateEnemies();

	//init timer for enemy attacking player
	attackTimer = attackTimeMax;


	HighScores scores;
	scores.LoadScores();
	if (scores.IsEmpty())
	{
		highScore = 0;
	}
	else
	{
		highScore = scores.GetHighScore();
	}
}

/*
called by machine state each frame
*/
void GameState::Update(float a_timestep, StateMachine* a_SMPointer)
{
	//if here and player dead must be more lives so run timer to start again
	if (!player->alive)
	{
		//run pause timer here
		if (currentRestartTime < restartTimer)
		{
			currentRestartTime += a_timestep;
		}
		else
		{
			currentRestartTime = 0;
			player->alive = true;
		}
	}

	//only send attacks when player alive and previous attack done
	if (player->alive && !sendAttack)
	{
		//will set sendAttack to true if it's time
		TimerTick(a_timestep);


		//if timer set attack this will run once per attack
		if (sendAttack)
		{
			//get min max column positions to qualify enemies for attack
			GetColExtremes(enemyColMinX, enemyColMaxX);

			//get random direction to attack from to qualify enemies for attack
			GetAttackDirection();

			//choose enemies qualified for attack
			ChooseAttackers();

			//set the leader
			SetAttackLeader();
		}
	}


	if (sendAttack)
	{
		sendNextEnemy();
		//check if attack run is complete or all attacking enemies dead
		if (lastEnemyReturned())
		{
			sendAttack = false;
		}
	}

	bool reverseGroup = false;
	//update enemy group positions
	for (float& colPos : enemyColPositions)
	{
		colPos += enemyGroupVelocity.x * enemyGroupSpeed * a_timestep;
		//check if need to reverse
		if (colPos > screenWidth * 0.85f)
		{
			colPos = screenWidth* 0.85f;
			reverseGroup = true;
		}
		else if (colPos < screenWidth * 0.15f)
		{
			colPos = screenWidth * 0.15f;
			reverseGroup = true;
		}
	}
	if (reverseGroup)
	{
		enemyGroupVelocity.x *= -1;
	}


	//new level flag
	bool allEnemyDead = true;
	for (auto object : gameObjects)
	{
		if (dynamic_cast<Enemy*>(object) != 0)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(object);

			enemy->Update(a_timestep, this);

			if (enemy->alive)
			{
				EnemyLogic(enemy, a_timestep);
				allEnemyDead = false;
			}

		}

		if (dynamic_cast<Player*>(object) != 0)
		{
			object->Update(a_timestep);

			//this may set gameOver flag so check after returns
			PlayerLogic(dynamic_cast<Player*>(object), a_timestep);
			if (gameOver)
			{
				//add score to high score list and then sort and save
				HighScores scores;
				scores.LoadScores();
				scores.AddScore(BaseState::score);
				scores.SortScores();
				scores.SaveScores();

				a_SMPointer->SwitchState(new EndGameState());
				return;
			}

		}

	}
	//next level condition
	if (allEnemyDead)
	{
		//recycle the player restart timer
		if (currentRestartTime >= restartTimer)
		{
			currentRestartTime = 0.0f;
			NewLevelInit();
		}
		else
		{
			currentRestartTime += a_timestep;
		}

	}

	BulletManager::Update(a_timestep);
}

/*
called by machine state each frame
*/
void GameState::Draw()
{
	DrawUI();

	for (auto object : gameObjects)
	{
		object->Draw();
	}
}

/*
this is called by state machine when this state is removed from stack, cleans up objects on
the heap instead of class destructor
*/
void GameState::Destroy()
{
	for (auto object : gameObjects)
	{
		DestroySprite(object->GetSpriteID());
		delete object;
	}
}

//static function to get position in grid
Point2d GameState::GetEnemyGroupPosition(int colIndex, int rowIndex)
{
	return Point2d(enemyColPositions[colIndex], enemyRowPositions[rowIndex]);
}

//private functions

/*
Creates each enemy object, setting their position in the group, and loading them in game objects array
*/
void GameState::CreateEnemies()
{
	//first enemy's position
	float enemyX = screenWidth * 0.2f;
	float enemyY = screenHeight *0.8f;

	//load cols x positions
	for (int colIndex = 0; colIndex < NUM_ENEMY_COLS; colIndex++)
	{
		if (enemyX > screenWidth * 0.8f)
		{
			//no more room so quit cols
			break;
		}
		enemyColPositions.push_back(enemyX);

		//increment to next col by enemy width + padding
		enemyX += 35 + 10.0f;
	}

	//load rows y positions 
	for (int rowIndex = 0; rowIndex < NUM_ENEMY_ROWS; rowIndex++)
	{
		enemyRowPositions.push_back(enemyY);
		//minus enemy height
		enemyY -= 25;
	}

	//create enemies with indexing
	for (int colIndex = 0; colIndex < enemyColPositions.size(); colIndex++)
	{
		for (int rowIndex = 0; rowIndex < enemyRowPositions.size(); rowIndex++)
		{
			Enemy* enemy = new Enemy("./images/blue_enemy/blue_enemy_1.png", 35, 25);
			//initialize position
			enemy->Init(Point2d(enemyColPositions[colIndex], enemyRowPositions[rowIndex]), Point2d(1, 0), 25, 30, 10.0f, colIndex, rowIndex);

			enemy->SetScoreValue(30);
			enemy->player = dynamic_cast<Player*>(gameObjects[0]);

			gameObjects.push_back(enemy);
		}
	}
}

/*
enemy group moving logic, enemy bullet collision
*/
void GameState::EnemyLogic(Enemy* enemy, float timeDelta)
{
	if (!enemy->isAttacking)
	{
		enemy->position = Point2d(enemyColPositions[enemy->colPositionIndex], enemyRowPositions[enemy->rowPositionIndex]);
	}

	if (enemy->isAttacking && enemy->GetAttackState() != ATTACK)
	{
		enemy->position.x += enemyGroupVelocity.x * enemyGroupSpeed * timeDelta;
	}

	//player bullet collision logic
	if (BulletManager::playerBullet->alive && enemy->alive && BulletManager::playerBullet->collider.isCollided(enemy->collider))
	{

		enemy->alive = false;

		BulletManager::playerBullet->alive = false;

		BaseState::score += enemy->health;
		if (BaseState::score > highScore)
			highScore = BaseState::score;
	}
}

/*
bullet player collision logic, and enemy player collision logic
*/
void GameState::PlayerLogic(Player* a_player, float a_delta)
{

	if (a_player->alive)
	{
		//check if player hit by bullet
		for (Bullet* enemyBullet : BulletManager::enemyBullets)
		{
			if (enemyBullet->alive)
			{
				if (enemyBullet->collider.isCollided(a_player->collider))
				{
					enemyBullet->alive = false;
					a_player->alive = false;
					PlayerDeath(a_player);
					break;
				}
			}

		}
		//check if player hit by enemy
		for (Entity* entity : gameObjects)
		{
			if (dynamic_cast<Enemy*>(entity) != 0)
			{
				Enemy* enemy = dynamic_cast<Enemy*>(entity);
				if (enemy->alive && enemy->isAttacking)
				{
					if (a_player->collider.isCollided(enemy->collider))
					{
						enemy->alive = false;
						a_player->alive = false;
						PlayerDeath(a_player);
						break;
					}
				}
			}
		}
	}
}

/*
Handle player after collision with enemy or enemy bullet.
will switch state to game over if was last life otherwise will decrement lives remaining
*/
void GameState::PlayerDeath(Player* player)
{
	if (playerLives > 1)
	{
		playerLives--;
	}
	else
	{
		gameOver = true;
	}
}

/*
Helper function for attacking enemies
*/
void GameState::ChooseAttackers()
{
	float delta = .01f;
	for (auto entity : gameObjects)
	{
		if (dynamic_cast<Enemy*>(entity) != 0)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(entity);
			//if direction is left (1) then use the min column else use the max
			if ((attackVelocity.x == 1 && Helper::FloatEquals(enemy->position.x, enemyColMinX, delta) ||
				(attackVelocity.x == -1 && Helper::FloatEquals(enemy->position.x, enemyColMaxX, delta))))
			{
				enemy->isAttacking = true;
				enemy->attackVelocity = attackVelocity;
				attackingEnemies.push_back(enemy);
			}

		}
	}

}

/*
Helper function for attacking enemies
this will set all enemies in array to attack state of WAIT unless it's the first enemy
constraint: attackingEnemies vector must have size > 0.
This function runs once per attack cycle;
*/
void GameState::SetAttackLeader()
{
	assert(attackingEnemies.size() > 0);

	for (int i = 0; i < attackingEnemies.size(); i++)
	{
		if (i != 0)
		{
			attackingEnemies[i]->SetAttackState(WAIT);
		}
		else
		{
			attackingEnemy = attackingEnemies[i];
		}
	}

}

/*
returns true if the last alive enemy in attacking enemy list has returned to it's original position
or there are no alive attacking enemies	else returns false
*/
bool GameState::lastEnemyReturned()
{
	//if all attacking enemies are not attacking then clear vector return true
	Enemy* lastAlive = nullptr;
	Enemy* lastReturned = nullptr;
	for (std::vector<Enemy*>::iterator it = attackingEnemies.begin(); it != attackingEnemies.end(); it++)
	{
		Enemy* enemy = *it;
		if (enemy->alive && enemy->isAttacking)
		{
			return false;
		}
	}
	//no one alive and attacking so either all dead or all not attacking so clear list and return true
	attackingEnemies.clear();
	attackingEnemy = nullptr;
	return true;
}

/*Helper function for attacking enemies
will check if the currently attacking enemy has moved to ATTACK state and if it is will change attack state
of next enemy in list to MOVE if there is one.
This function ran every frame*/
void GameState::sendNextEnemy()
{
	//if attackingEnemy not in attack state return
	if (attackingEnemy->GetAttackState() != ATTACK)
		return;

	bool sendNext = false;
	for (std::vector<Enemy*>::iterator it = attackingEnemies.begin(); it != attackingEnemies.end(); it++)
	{
		Enemy* enemy = *it;
		if (sendNext)
		{
			attackingEnemy = enemy;
			enemy->SetAttackState(MOVE);
			return;
		}
		if (enemy == attackingEnemy)
		{
			sendNext = true;
		}

	}
}

/*
helper function for attacking enemies
*/
int GameState::GetRandomDirection()
{
	int dir = rand() % 2;
	if (dir = 0) return -1; //left
	else return 1;//right
}

/*
helper function for attacking enemies
returns max and min y position values for all enemies
*/
void GameState::GetEnemyColX(float& minX, float& maxX)
{
	minX = screenWidth;
	maxX = 0.0f;
	for (auto object : gameObjects)
	{
		if (dynamic_cast<Enemy*>(object) != 0)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(object);
			if (enemy->GetPosition().x > maxX)
			{
				maxX = enemy->GetPosition().x;
			}
			if (enemy->GetPosition().x < minX)
			{
				minX = enemy->GetPosition().x;
			}
		}
	}
}

/*
invert the velocity on every alive enemy
*/
void GameState::ReverseEnemies()
{
	for (auto object : gameObjects)
	{
		if (dynamic_cast<Enemy*>(object) != 0)
		{
			dynamic_cast<Enemy*>(object)->velocity.x = dynamic_cast<Enemy*>(object)->velocity.x * -1;
		}
	}
}

/*
Helper file for attacking player
*/
void GameState::TimerTick(float timeDelta)
{
	if (attackTimer >= 0)
	{
		attackTimer -= timeDelta;
	}
	else
	{
		attackTimer = attackTimeMax;
		sendAttack = true;
	}
}

/*
Helper function for attacking player
*/
void GameState::GetColExtremes(float& minX, float& maxX)
{
	//reset 
	minX = screenWidth;
	maxX = 0.0f;
	//loop through all enemies and find minX and maxX positions and set parameters to same
	for (auto object : gameObjects)
	{

		if (dynamic_cast<Enemy*>(object) != 0)
		{
			float enemyX = dynamic_cast<Enemy*>(object)->GetPosition().x;
			if (enemyX < minX)
			{
				minX = enemyX;
			}
			if (enemyX > maxX)
			{
				maxX = enemyX;
			}
		}
	}
}

void GameState::GetAttackDirection()
{
	int i = rand() % 2; //0 or 1
	if (i == 0)
	{
		attackVelocity = Point2d(-1, 1); //right circle
	}
	else
	{
		attackVelocity = Point2d(1, 1);//left circle
	}
}

void GameState::DrawUI()
{

	DrawString(scoreLabel, scorePos.x, scorePos.y);
	sprintf(scoreAsString, "%05d", BaseState::score);
	DrawString(scoreAsString, scorePos.x, scorePos.y - 25, SColour(255, 0, 0, 255));

	DrawString(highScoreLabel, highScorePos.x, highScorePos.y);
	sprintf(HighScoreAsString, "%05d", highScore);
	DrawString(HighScoreAsString, screenWidth * .5f, highScorePos.y - 25, SColour(255, 0, 0, 255));

	//player lives
	int paddingX = 40.0f;
	//start i at 1 because first life is playing
	for (int i = 1; i < playerLives; i++)
	{
		MoveSprite(playerLifeTextureID, screenWidth * .05f + (paddingX * i), 25.0f);
		DrawSprite(playerLifeTextureID);
	}

}

/*
initialize objects for new level. called when all enemy dead
*/
void GameState::NewLevelInit()
{
	//first enemy's position
	float enemyX = screenWidth * 0.2f;
	float enemyY = screenHeight *0.8f;

	//init enemy
	for (Entity* entity : gameObjects)
	{
		if (dynamic_cast<Enemy*>(entity) != 0)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(entity);
			enemy->alive = true;
			enemy->isAttacking = false;

			enemy->SetAttackState(MOVE);
			enemy->SetAttackAngle(0.0f);
			enemy->attackExitChosen = false;
			enemy->SetAttackExitPoint(Point2d());
			enemy->attackSlope = 0.0f;
			enemy->attackYIntercept = 0.0f;
			enemy->attackVelocity = Point2d();

			//check if need new line of enemy
			if (enemyX > screenWidth * 0.8f)
			{
				enemyX = screenWidth * 0.2f;
				enemyY -= enemy->GetHeight();
			}

			enemy->position = Point2d(enemyX, enemyY);

			//increment next enemy's x position
			enemyX += enemy->GetWidth() + 10.0f;
		}
	}

	sendAttack = false;

}

char* GameState::ScoreToString(int a_score)
{
	char result[20];
	sprintf(result, "%05d", a_score);
	return result;
}
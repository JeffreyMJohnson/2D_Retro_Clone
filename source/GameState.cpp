#include "GameState.h"




//global variables for position calculating
extern const int screenWidth;
extern const int screenHeight;

//PUBLIC FUNCTIONS

GameState::GameState()
{
	scoreLabel = "1UP";
	highScoreLabel = "HIGH SCORE";
	scorePos = { screenWidth * .1f, screenHeight };
	highScorePos = { screenWidth * .5f - 75.0f, screenHeight };
	score = 0;
	highScore = 10000;
	playerLives = 3;
	playerLifeTextureID = CreateSprite("./images/misc/user_life_sprite.png", 35.0f, 41.0f, true);
	srand(time(nullptr));
	sendAttack = false;
	enemyColMinX = 0.0f;
	enemyColMaxX = 0.0f;

	//need to be a positive y for first stage of attack
	attackVelocity = Point2d{ -1, 1 };


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
	//gameOver = false; 

	//initialize bulletManager static class
	BulletManager::Init();

	Player* player = new Player("./images/player/galaxian.png", 45.0f, 51.0f);
	player->Init(Point2d{ screenWidth * 0.5f, 100.0f }, Point2d{ 0, 0 }, 10.0f, 1);
	
	gameObjects.push_back(player);

	//create and position enemy group
	CreateEnemies();

	//init timer for enemy attacking player
	attackTimer = attackTimeMax;

	//need?
	fontFile = "./fonts/galaxian.fnt";

	
	//Highscores scores;
	//scores.LoadScores();
	//if (scores.IsEmpty())
	//{
	//	highScore = 0;
	//}
	//else
	//{
	//	scores.SortScores();
	//	highScore = *scores.GetScores().begin();
	//}
}

/*
called by machine state each frame
*/
void GameState::Update(float a_timestep, StateMachine* a_SMPointer)
{
	if (!sendAttack)
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
		if (attackingEnemies.size() == 2 && attackingEnemies[1]->GetAttackState() == WAIT && attackingEnemies[0]->GetAttackState() == ATTACK)
		{
			attackingEnemies[1]->SetAttackState(MOVE);
		}
	}

	for (auto object : gameObjects)
	{
		object->Update(a_timestep);
		if (dynamic_cast<Enemy*>(object) != 0)
		{
			EnemyLogic(dynamic_cast<Enemy*>(object), a_timestep);
		}

		if (dynamic_cast<Player*>(object) != 0)
		{
			PlayerLogic(dynamic_cast<Player*>(object), a_timestep);


		}

	}

	BulletManager::Update(a_timestep);


	////escape key
	//if (IsKeyDown(256))
	//{
	//	BaseState* lastState = a_SMPointer->PopState();
	//	delete lastState;
	//	return;
	//}

	//if (gameOver)
	//{
	//	gameOverTimer -= a_timestep;
	//	if (gameOverTimer <= 0)
	//	{
	//		LeaderboardState* leaderBoard = new LeaderboardState();
	//		leaderBoard->SetPlayersScore(score);
	//		BaseState* lastState = a_SMPointer->SwitchState(leaderBoard);
	//		delete lastState;
	//	}
	//	return;
	//}

	/*if (allDead)
	{
	gameOver = true;
	return;
	}*/



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
	//DestroySprite(bulletTexture);
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

	for (int i = 0; i < NUM_ENEMYS; i++)
	{

		Enemy* enemy = new Enemy("./images/blue_enemy/blue_enemy_1.png", 35, 25);

		//check if need new line of enemy
		if (enemyX > screenWidth * 0.8f)
		{
			enemyX = screenWidth * 0.2f;
			enemyY -= enemy->GetHeight();
		}

		//initialize position
		enemy->Init(Point2d{ enemyX, enemyY }, Point2d{ 1, 0 }, 25, 30, 1.0f);

		//increment next enemy's x position
		enemyX += enemy->GetWidth() + 10.0f;

		enemy->SetScoreValue(30);

		enemy->player = dynamic_cast<Player*>(gameObjects[0]);

		gameObjects.push_back(enemy);
	}
}

/*
enemy group moving logic, enemy bullet collision
*/
void GameState::EnemyLogic(Enemy* enemy, float timeDelta)
{
	enemy->Update(timeDelta);

	//enemies who aren't attacking  moving left and right max and min logic
	if (enemy->GetPosition().x > screenWidth * 0.85f && !enemy->isAttacking)
	{
		enemy->SetX(screenWidth * 0.85f);
		ReverseEnemies();
	}//use return position so it keeps it's spot
	else if (enemy->isAttacking && enemy->GetReturnPosition().x > screenWidth * 0.85f)
	{
		ReverseEnemies();
	}
	else if (enemy->GetPosition().x < screenWidth * 0.15f && !enemy->isAttacking)
	{
		enemy->SetX(screenWidth * 0.15f);

		ReverseEnemies();
	}
	else if (enemy->isAttacking && enemy->GetReturnPosition().x < screenWidth * 0.15f)
	{
		ReverseEnemies();
	}

	//player bullet collision logic
	if (BulletManager::playerBullet->alive && enemy->alive && BulletManager::playerBullet->collider.isCollided(enemy->collider))
	{
		enemy->alive = false;
		BulletManager::playerBullet->alive = false;

		//need to add score here
	}
}

/*
bullet player collision logic, and enemy player collision logic
*/
void GameState::PlayerLogic(Player* a_player, float a_delta)
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

					//TODO::next life code here

				}
			}
		}
	}
}

/*
Helper function for attacking enemies
*/
void GameState::ChooseAttackers()
{

	for (auto entity : gameObjects)
	{
		if (dynamic_cast<Enemy*>(entity) != 0)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(entity);
			//if direction is left (1) then use the min column else use the max
			if ((attackVelocity.x == 1 && enemy->position.x == enemyColMinX) ||
				(attackVelocity.x == -1 && enemy->position.x == enemyColMaxX))
			{
				enemy->isAttacking = true;
				enemy->attackVelocity = attackVelocity;
				enemy->SetReturnPosition(enemy->position);
				attackingEnemies.push_back(enemy);
			}

		}
	}

}

/*
Helper function for attacking enemies
this will set the isLeader flag on enemy with highest y value of attacking enemies
constraint: attackingEnemies vector must have size > 0.
*/
void GameState::SetAttackLeader()
{
	assert(attackingEnemies.size() > 0);

	for (int i = 0; i < attackingEnemies.size(); i++)
	{
		if (i == 0)
		{
			attackingEnemies[i]->isLeader = true;
		}
		else
		{
			attackingEnemies[i]->isLeader = false;
			attackingEnemies[i]->SetAttackState(WAIT);
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
		attackVelocity = Point2d{ -1, 1 }; //right circle
	}
	else
	{
		attackVelocity = Point2d{ 1, 1 };//left circle
	}
}

void GameState::DrawUI()
{

	SetFont(fontFile.c_str());
	//1up string above score;
	//SetFont("./fonts/galaxian.fnt");
	DrawString(scoreLabel, scorePos.x, scorePos.y);
	sprintf(scoreAsString, "%05d", score);
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

char* GameState::ScoreToString(int a_score)
{
	char result[20];
	sprintf(result, "%05d", a_score);
	return result;
}
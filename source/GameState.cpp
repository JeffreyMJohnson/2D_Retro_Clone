#include "GameState.h"
#include <iostream>
#include <string>




extern const int screenWidth;
extern const int screenHeight;



//extern const char* invadersFont;

//extern const char* player2ScoreText;
//extern const char* insertCoinsText;
//extern const char* creditText;
//
//extern char* player2Score;

//extern char* playerLives;
//extern char* credit;
//
//extern const float lineYPos;


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
	attackDirection = -1;
}


GameState::~GameState()
{
	delete bullet;
}

void GameState::Initialize()
{
	//gameOver = false; 

	Player* player = new Player();

	player->SetSize(45.0f, 51.0f);
	player->SetMovementKeys('A', 'D');
	//player->SetShootKey(32); //space key
	player->SetMovementExtremes(0, screenWidth);
	player->SetSpriteId(CreateSprite("./images/player/galaxian.png", player->GetWidth(), player->GetHeight(), true));
	player->SetPosition(screenWidth * 0.5f, 100.0f);
	player->SetSpeed(10.0f);

	////add player to dynamic array
	gameObjects.push_back(player);

	MoveSprite(player->GetSpriteID(), player->GetPosition().x, player->GetPosition().y);


	bullet = new Bullet();
	bullet->height = 15.0f;
	bullet->width = 4.0f;
	bulletYOffset = bullet->height * 2;
	bullet->x = player->GetPosition().x;
	bullet->y = player->GetPosition().y + bulletYOffset;
	bullet->velocityY = 100.0f;
	bullet->textureId = CreateSprite("./images/bullet.png", bullet->width, bullet->height, true);

	//bulletTexture = CreateSprite("./images/player_shot.png", 3, 20, true);

	CreateEnemies();

	attackTimer = attackTimeMax;

	fontFile = "./fonts/galaxian.fnt";

	//AddFont(fontFile.c_str());

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

void GameState::Destroy()
{
	for (auto object : gameObjects)
	{
		DestroySprite(object->GetSpriteID());
		delete object;
	}
	//DestroySprite(bulletTexture);
}



void GameState::EnemyLogic(Enemy* enemy, float timeDelta)
{
	//bool isAttacking = enemy->isAttacking;


	//enemies who aren't attacking  moving left and right max and min logic
	if (enemy->GetPosition().x > screenWidth * 0.85f && !enemy->isAttacking)
	{
		enemy->SetX(screenWidth * 0.85f);
		ReverseEnemies();
	}
	else if (enemy->GetPosition().x < screenWidth * 0.15f && !enemy->isAttacking)
	{
		enemy->SetX(screenWidth * 0.15f);

		ReverseEnemies();
	}

	//enemy bullet collision logic
	if (enemy->GetIsActive() && bullet->isCollided(enemy))
	{
		enemy->SetIsActive(false);
		bullet->isActive = false;
	}

	//attack logic


	//if (sendAttack && enemy->isAttacking && enemy->GetAttackState() == ATTACK && enemy->attackExitChosen == false)
	//{
	//		float origX = enemy->GetreturnPosition().x;
	//		float origY = enemy->GetreturnPosition().y;
	//		float b = 0;
	//		float m = 0;

	//	//get a player handle
	//	Player* player = dynamic_cast<Player*>(gameObjects[0]);
	//	assert(player != 0);

	//	//pick point to fly to 
	//	//		//left or right of player  if enemy on player left will fly to right and vice versa
	//	if (enemy->GetPosition().x < player->GetPosition().x)
	//	{
	//		//pick to the right of the player 
	//		Point2d exit{ 0, 0 };
	//		exit.x = player->GetPosition().x + 100.0f;
	//		enemy->SetAttackExitPoint(exit);
	//		enemy->attackExitChosen = true;
	//	}
	//	else if (!enemy->attackExitChosen) //enemy to right or equal of player so go left
	//	{
	//		Point2d exit{ 0, 0 };
	//		exit.x = player->GetPosition().x - 100.0f;
	//		enemy->SetAttackExitPoint(exit);
	//		enemy->attackExitChosen = true;
	//	}

	//	//TODO: get this refactored so only call once per attack sequence
	//	//slope formula - m = (y1 -y2) / (x1 - x2)
	//	m = GetSlopeOfLine(enemy->GetPosition(), enemy->GetAttackExitPoint());

	//	//TODO: get this refactored so only call once per attack sequence
	//	//point-slope formula - y - y1 = m * (x - x1) choose any point on line as (x1, y1)
	//	//b = y - m * x
	//	//y = m * x + b
	//	b = enemy->GetPosition().y - (m * enemy->GetPosition().x);

	//	//increment x
	//	if (enemy->GetPosition().y > enemy->GetAttackExitPoint().y && enemy->GetPosition().x < enemy->GetAttackExitPoint().x)
	//	{
	//		float x = enemy->GetPosition().x + enemy->GetSpeed() * timeDelta;
	//		float y = (m * x) + b;
	//		enemy->SetPosition(x, y);
	//	}
	//	//decrement x
	//	else if (enemy->GetPosition().y > enemy->GetAttackExitPoint().y && enemy->GetPosition().x > enemy->GetAttackExitPoint().x)
	//	{
	//		float x = enemy->GetPosition().x - enemy->GetSpeed() * timeDelta;
	//		float y = (m * x) + b;
	//		enemy->SetPosition(x, y);
	//	}
	//	else
	//	{
	//		enemy->attackExitChosen = false;

	//		//set enemy x to original position and y to screenheight
	//		enemy->SetPosition(enemy->GetreturnPosition().x, screenHeight);
	//		enemy->SetAttackState(RETURN);
	//	}

	//	//TODO: turn bool attackExitIsChosen to false before leaving this state
	//	//break;
	//}

	//this should be moved to enemy class use function pointers for context update
	//if (isAttacking)
	//{
	//	float origX = enemy->GetreturnPosition().x;
	//	float origY = enemy->GetreturnPosition().y;
	//	float b = 0;
	//	float m = 0;

	//	switch (enemy->GetAttackState())
	//	{
	//	case MOVE:
	//		//check if returnPosition still initialized state
	//		if (origX == 0 && origY == 0)
	//		{
	//			Point2d originalPos;
	//			originalPos.x = enemy->GetPosition().x;
	//			originalPos.y = enemy->GetPosition().y;
	//			enemy->SetOriginalPos(originalPos);
	//			//origX = enemy->GetreturnPosition().x;
	//			//origY = enemy->GetreturnPosition().y;
	//		}
	//		//move enemy distance of y = attackRadius + returnPosition.y
	//		if (enemy->GetPosition().y < (enemy->GetreturnPosition().y + enemy->GetAttackRadius()))
	//		{
	//			enemy->SetY(enemy->GetPosition().y + enemy->GetSpeed() * timeDelta);
	//		}
	//		else
	//		{
	//			enemy->SetAttackState(CIRCLE);
	//		}
	//		break;
	//	case CIRCLE:
	//		//attackAngle is in radians convert to be in degrees
	//		if (enemy->GetAttackAngle() <= enemy->DegreeToRadians(270.0f))
	//		{
	//			float x = (enemy->GetreturnPosition().x + (enemy->GetAttackRadius() * cos(enemy->GetAttackAngle())));
	//			float y = (enemy->GetreturnPosition().y + (enemy->GetAttackRadius() * sin(enemy->GetAttackAngle())));
	//			float cosA = cos(enemy->GetAttackAngle());
	//			float sinA = sin(enemy->GetAttackAngle());
	//			float angleR = enemy->GetAttackAngle();
	//			float angleD = enemy->RadiansToDegrees(enemy->GetAttackAngle());
	//			enemy->SetAttackAngle(enemy->GetAttackAngle() + enemy->DegreeToRadians(.1));

	//			enemy->SetPosition(x, y);
	//		}
	//		else
	//		{
	//			enemy->SetAttackAngle(enemy->DegreeToRadians(90.0f));
	//			enemy->SetAttackState(ATTACK);
	//		}
	//		break;
	//	case ATTACK:

	//		//get player handle
	//		Player* player;
	//		for (auto object : gameObjects)
	//		{
	//			if (dynamic_cast<Player*>(object))
	//			{
	//				player = dynamic_cast<Player*>(object);
	//				break;
	//			}
	//		}
	//		//pick point to fly to 
	//		//left or right of player  if enemy on player left will fly to right and vice versa
	//		if (!enemy->attackExitChosen && enemy->GetPosition().x < player->GetPosition().x)
	//		{
	//			//pick to the right of the player 
	//			Point2d exit{ 0, 0 };
	//			exit.x = player->GetPosition().x + 100.0f;
	//			enemy->SetAttackExitPoint(exit);
	//			enemy->attackExitChosen = true;
	//		}
	//		else if (!enemy->attackExitChosen) //enemy to right or equal of player so go left
	//		{
	//			Point2d exit{ 0, 0 };
	//			exit.x = player->GetPosition().x - 100.0f;
	//			enemy->SetAttackExitPoint(exit);
	//			enemy->attackExitChosen = true;
	//		}

	//		//TODO: get this refactored so only call once per attack sequence
	//		//slope formula - m = (y1 -y2) / (x1 - x2)
	//		m = GetSlopeOfLine(enemy->GetPosition(), enemy->GetAttackExitPoint());

	//		//TODO: get this refactored so only call once per attack sequence
	//		//point-slope formula - y - y1 = m * (x - x1) choose any point on line as (x1, y1)
	//		//b = y - m * x
	//		//y = m * x + b
	//		b = enemy->GetPosition().y - (m * enemy->GetPosition().x);

	//		//increment x
	//		if (enemy->GetPosition().y > enemy->GetAttackExitPoint().y && enemy->GetPosition().x < enemy->GetAttackExitPoint().x)
	//		{
	//			float x = enemy->GetPosition().x + enemy->GetSpeed() * timeDelta;
	//			float y = (m * x) + b;
	//			enemy->SetPosition(x, y);
	//		}
	//		//decrement x
	//		else if (enemy->GetPosition().y > enemy->GetAttackExitPoint().y && enemy->GetPosition().x > enemy->GetAttackExitPoint().x)
	//		{
	//			float x = enemy->GetPosition().x - enemy->GetSpeed() * timeDelta;
	//			float y = (m * x) + b;
	//			enemy->SetPosition(x, y);
	//		}
	//		else
	//		{
	//			enemy->attackExitChosen = false;

	//			//set enemy x to original position and y to screenheight
	//			enemy->SetPosition(enemy->GetreturnPosition().x, screenHeight);
	//			enemy->SetAttackState(RETURN);
	//		}

	//		//TODO: turn bool attackExitIsChosen to false before leaving this state
	//		break;
	//	case RETURN:
	//		//return to original position
	//		//std::cout << "foo";

	//		if (enemy->GetPosition().y > enemy->GetreturnPosition().y)
	//		{
	//			float y = enemy->GetPosition().y;
	//			float returnY = enemy->GetreturnPosition().y;
	//			enemy->SetPosition(enemy->GetPosition().x, enemy->GetPosition().y - enemy->GetSpeed() * timeDelta);
	//		}
	//		else
	//		{
	//			enemy->isAttacking = false;
	//			enemy->SetAttackState(MOVE);
	//			std::cout << "done";
	//		}

	//		break;
	//	}
	//}
}

/*sort the attackingEnemies vector in descending order of y position
require: the vector must have at least 1 entity and at most 2 in the vector
*/
//void GameState::SortAttackingEnemies()
//{
//	assert(attackingEnemies.size() > 0 && attackingEnemies.size() < 3);
//	if (attackingEnemies.size() == 1)
//		return;
//	if (attackingEnemies[0]->GetPosition().y > attackingEnemies[1]->GetPosition().y)
//	{
//		Entity* t = attackingEnemies[0];
//		attackingEnemies[0] = attackingEnemies[1];
//		attackingEnemies[1] = t;
//	}
//}

void GameState::ChooseAttackers()
{

	for (auto entity : gameObjects)
	{
		if (dynamic_cast<Enemy*>(entity) != 0)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(entity);
			//if direction is left (1) then use the min column else use the max
			if ((attackDirection == 1 && enemy->GetPosition().x == enemyColMinX) ||
				(attackDirection == -1 && enemy->GetPosition().x == enemyColMaxX))
			{
				enemy->isAttacking = true;
				enemy->attackDirection = attackDirection;
				enemy->SetReturnPosition(enemy->GetPosition());
				attackingEnemies.push_back(enemy);
			}

		}
	}



	//float minX = screenWidth;
	//float maxX = 0.0f;

	//GetEnemyColX(minX, maxX);

	//int attackDirection = GetRandomDirection();

	//for (auto object : gameObjects)
	//{
	//	if (dynamic_cast<Enemy*>(object) != 0)
	//	{
	//		Enemy* enemy = dynamic_cast<Enemy*>(object);

	//		//going to attack to left so use minx
	//		if (enemy->GetPosition().x == minX && enemy->attackDirection == -1)
	//		{
	//			enemy->isAttacking = true;
	//		}


	//		//going to attack to left so use maxX
	//		if (enemy->GetPosition().x == maxX && enemy->GetDirection() == 1)
	//		{
	//			enemy->isAttacking = true;
	//		}
	//	}
	//}

	//sendAttack = true;

}

/*
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

int GameState::GetRandomDirection()
{
	int dir = rand() % 2;
	if (dir = 0) return -1; //left
	else return 1;//right
}

//returns max and min y position values for all enemies
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

void GameState::ReverseEnemies()
{
	for (auto object : gameObjects)
	{
		if (dynamic_cast<Enemy*>(object) != 0)
		{
			dynamic_cast<Enemy*>(object)->SetDirection(dynamic_cast<Enemy*>(object)->GetDirection() * -1);
		}
	}
}

//void GameState::PlayerLogic(Player* a_player, float a_delta)
//{
//	a_player->Shoot(bulletTexture, a_delta);
//
//	for (int i = 0; i < MAX_BULLETS; i++)
//	{
//		a_player->bullets[i].Update(a_delta);
//		a_player->bullets[i].Draw();
//	}
//
//	for (auto enemy : gameObjects)
//	{
//		if (dynamic_cast<Enemy*>(enemy) != 0)
//		{
//			Enemy* enemyShip = dynamic_cast<Enemy*>(enemy);
//			for (int i = 0; i < MAX_BULLETS; i++)
//			{
//				if (CheckCollision(a_player->bullets[i].x, a_player->bullets[i].y, enemyShip->GetPosition().x, enemyShip->GetPosition().y, 30.0f) &&
//					enemyShip->GetIsActive() &&
//					a_player->bullets[i].isActive)
//				{
//					enemyShip->SetIsActive(false);
//					a_player->bullets[i].isActive = false;
//					score += enemyShip->GetScoreValue();
//					activeEnemiesCount--;
//				}
//			}
//		}
//	}
//}
//
//void GameState::EnemyLogic(Enemy* a_enemy, bool& lowerAliens)
//{
//	if (a_enemy->GetPosition().x > screenWidth * 0.9f && !lowerAliens)
//	{
//		direction = -1;
//		lowerAliens = true;
//		//break;
//	}
//	else if (a_enemy->GetPosition().x < screenWidth * 0.1f && !lowerAliens)
//	{
//		direction = 1;
//		lowerAliens = true;
//		//break;
//	}
//
//	int speed = 750;
//	a_enemy->SetDirection(direction);
//	//check for zero before dividing damn it!
//	if (activeEnemiesCount != 0)
//		speed /= activeEnemiesCount;
//	a_enemy->SetSpeed(speed);
//}

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
		attackDirection = -1; //right circle
	}
	else
	{
		attackDirection = 1;//left circle
	}
	//debug
	attackDirection = -1;
}

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
			Player* player = dynamic_cast<Player*>(object);
			if (IsKeyDown(player->shootKey) && !bullet->isActive)
			{
				bullet->isActive = true;
			}

			if (bullet->isActive)
			{
				bullet->Update(a_timestep);
			}
			else
			{
				bullet->x = player->GetPosition().x;
				bullet->y = player->GetPosition().y + bulletYOffset;
			}
		}

	}




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

	//update enemies
	//bool lowerAliens = false;
	//bool allDead = true;

	//for (auto object : gameObjects)
	//{
	//	//determine the type at runtime
	//	if (dynamic_cast<Player*>(object) != 0)
	//	{
	//		//process player specific  logic
	//		PlayerLogic(dynamic_cast<Player*>(object), a_timestep);
	//	}

	//if (dynamic_cast<Enemy*>(object) != 0)
	//{
	//	//process enemy specific logic
	//	Enemy* enemy = dynamic_cast<Enemy*>(object);
	//	EnemyLogic(enemy, lowerAliens);
	//	if (enemy->GetIsActive())
	//	{
	//		allDead = false;
	//	}
	//}

	//	//update and draw objects
	//	object->Update(a_timestep);
	//	object->Draw();
	//}

	/*if (allDead)
	{
	gameOver = true;
	return;
	}*/

	//with better logic this could be put in the main object update loop 
	/*if (lowerAliens)
	{
	for (auto object : gameObjects)
	{
	if (dynamic_cast<Enemy*>(object) != 0)
	{
	Enemy* enemy = dynamic_cast<Enemy*>(object);

	if (enemy->GetPosition().y <= (0.05f * screenHeight))
	{
	gameOver = true;
	gameOverTimer = 2;
	return;
	}

	enemy->SetY(enemy->GetPosition().y - (0.05f * screenHeight));
	}
	}
	}*/

}

void GameState::Draw()
{
	DrawUI();

	for (auto object : gameObjects)
	{
		object->Draw();
	}

	bullet->Draw();

	//DrawLine(0, lineYPos, screenWidth, lineYPos, SColour(0x00, 0xFF, 0x00, 0xFF));

	//SetFont(invadersFont);
	//sprintf(p1Score_s, "%05d", score);
	//DrawString(player1ScoreText, screenWidth * 0.025f, screenHeight - 2);
	//DrawString(p1Score_s, screenWidth * 0.025f, screenHeight - 25);

	//sprintf(highScore_s, "%05d", highScore);
	//DrawString(highScoreText, (screenWidth / 2) - 90, screenHeight - 2);
	//DrawString(highScore_s, screenWidth / 2 - 60, screenHeight - 25);

	//DrawString(player2ScoreText, screenWidth - 150, screenHeight - 2);
	///*DrawString(scoreAsString.c_str(), 35, screenHeight - 30);
	//DrawString(player2Score, screenWidth - 125, screenHeight - 30);*/

	//DrawString(playerLives, 40, lineYPos - 2);
	//DrawString(creditText, screenWidth - 200, lineYPos - 2);
	//DrawString(credit, screenWidth - 75, lineYPos - 2);


}
bool GameState::CheckCollision(float x1, float y1, float x2, float y2, float distance)
{
	float d = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	if (d < distance)
		return true;
	else
		return false;
}

void GameState::CreateEnemies()
{
	//first enemy's position
	float enemyX = screenWidth * 0.2f;
	float enemyY = screenHeight *0.75f;

	for (int i = 0; i < NUM_ENEMYS; i++)
	{

		Enemy* enemy = new Enemy();

		//enemy->SetSize(58, 26);
		enemy->SetSize(35.0f, 25.0f);
		enemy->SetSpeed(1.0f);

		enemy->SetSpriteId(CreateSprite("./images/blue_enemy/blue_enemy_1.png", enemy->GetWidth(), enemy->GetHeight(), true));

		//check if need new line of enemy
		if (enemyX > screenWidth * 0.8f)
		{
			enemyX = screenWidth * 0.2f;
			enemyY -= enemy->GetHeight();
		}

		//initialize position
		enemy->SetPosition(enemyX, enemyY);

		//increment next enemy's x position
		enemyX += enemy->GetWidth() + 10.0f;

		enemy->SetScoreValue(30);

		enemy->player = dynamic_cast<Player*>(gameObjects[0]);

		gameObjects.push_back(enemy);
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
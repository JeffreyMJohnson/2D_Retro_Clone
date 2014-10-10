#include "GameState.h"
#include <iostream>



extern const int screenWidth;
extern const int screenHeight;

//extern const char* invadersFont;
//extern const char* player1ScoreText;
//extern const char* player2ScoreText;
//extern const char* highScoreText;
//extern const char* insertCoinsText;
//extern const char* creditText;
//
//extern std::string scoreAsString;
//extern char* player2Score;
//extern char* highScore;
//extern char* playerLives;
//extern char* credit;
//
//extern const float lineYPos;


GameState::GameState()
{
}


GameState::~GameState()
{
}

void GameState::Initialize()
{
	//gameOver = false; 

	Player* player = new Player();

	player->SetSize(64.0f, 32.0f);
	player->SetMovementKeys('A', 'D');
	//player->SetShootKey(32); //space key
	player->SetMovementExtremes(0, screenWidth);
	player->SetSpriteId(CreateSprite("./images/cannon.png", player->GetWidth(), player->GetHeight(), true));
	player->SetPosition(screenWidth * 0.5f, 50.0f);
	player->SetSpeed(10.0f);

	////add player to dynamic array
	gameObjects.push_back(player);

	MoveSprite(player->GetSpriteID(), player->GetPosition().x, player->GetPosition().y);

	//bulletTexture = CreateSprite("./images/player_shot.png", 3, 20, true);

	CreateEnemies();

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

float GetSlopeOfLine(Point2d point1, Point2d point2)
{
	return (point1.y - point2.y) / (point1.x - point2.x);

}

void GameState::EnemyLogic(Enemy* a_enemy, float timeDelta)
{
	bool isAttacking = a_enemy->isAttacking;


	//enemies moving left and right logic
	if (a_enemy->GetPosition().x > screenWidth * 0.95f)
	{
		a_enemy->SetX(screenWidth * 0.95f);
		ReverseEnemies();
	}
	else if (a_enemy->GetPosition().x < screenWidth * 0.05f)
	{
		a_enemy->SetX(screenWidth * 0.05f);
		ReverseEnemies();
	}

	//only use timer if not attacking
	if (!isAttacking && attackTimer > 0.0f)
	{
		attackTimer -= timeDelta;
	}
	else if (!isAttacking)
	{
		attackTimer = 50.0f;
		a_enemy->isAttacking = true;

	}

	//this should be moved to enemy class use function pointers for context update
	if (isAttacking)
	{
		float origX = a_enemy->GetOriginalPosition().x;
		float origY = a_enemy->GetOriginalPosition().y;
		float b = 0;
		float m = 0;

		switch (a_enemy->GetAttackState())
		{
		case MOVE:
			//check if originalPosition still initialized state
			if (origX == 0 && origY == 0)
			{
				Point2d originalPos;
				originalPos.x = a_enemy->GetPosition().x;
				originalPos.y = a_enemy->GetPosition().y;
				a_enemy->SetOriginalPos(originalPos);
				//origX = a_enemy->GetOriginalPosition().x;
				//origY = a_enemy->GetOriginalPosition().y;
			}
			//move enemy distance of y = attackRadius + originalPosition.y
			if (a_enemy->GetPosition().y < (a_enemy->GetOriginalPosition().y + a_enemy->GetAttackRadius()))
			{
				a_enemy->SetY(a_enemy->GetPosition().y + a_enemy->GetSpeed() * timeDelta);
			}
			else
			{
				a_enemy->SetAttackState(CIRCLE);
			}
			break;
		case CIRCLE:
			//attackAngle is in radians convert to be in degrees
			if (a_enemy->GetAttackAngle() <= a_enemy->DegreeToRadians(270.0f))
			{
				float x = (a_enemy->GetOriginalPosition().x + (a_enemy->GetAttackRadius() * cos(a_enemy->GetAttackAngle())));
				float y = (a_enemy->GetOriginalPosition().y + (a_enemy->GetAttackRadius() * sin(a_enemy->GetAttackAngle())));
				float cosA = cos(a_enemy->GetAttackAngle());
				float sinA = sin(a_enemy->GetAttackAngle());
				float angleR = a_enemy->GetAttackAngle();
				float angleD = a_enemy->RadiansToDegrees(a_enemy->GetAttackAngle());
				a_enemy->SetAttackAngle(a_enemy->GetAttackAngle() + a_enemy->DegreeToRadians(.1));

				a_enemy->SetPosition(x, y);
			}
			else
			{
				a_enemy->SetAttackState(ATTACK);
			}
			break;
		case ATTACK:

			//get player handle
			Player* player;
			for (auto object : gameObjects)
			{
				if (dynamic_cast<Player*>(object))
				{
					player = dynamic_cast<Player*>(object);
					break;
				}
			}
			//pick point to fly to 
			//left or right of player  if enemy on player left will fly to right and vice versa
			if (!a_enemy->attackExitChosen && a_enemy->GetPosition().x < player->GetPosition().x)
			{
				//pick to the right of the player 
				Point2d exit{ 0, 0 };
				exit.x = player->GetPosition().x + 100.0f;
				a_enemy->SetAttackExitPoint(exit);
				a_enemy->attackExitChosen = true;
			}
			else if (!a_enemy->attackExitChosen) //enemy to right or equal of player so go left
			{
				Point2d exit{ 0, 0 };
				exit.x = player->GetPosition().x - 100.0f;
				a_enemy->SetAttackExitPoint(exit);
				a_enemy->attackExitChosen = true;
			}

			//TODO: get this refactored so only call once per attack sequence
			//slope formula - m = (y1 -y2) / (x1 - x2)
			m = GetSlopeOfLine(a_enemy->GetPosition(), a_enemy->GetAttackExitPoint());

			//TODO: get this refactored so only call once per attack sequence
			//point-slope formula - y - y1 = m * (x - x1) choose any point on line as (x1, y1)
			//b = y - m * x
			//y = m * x + b
			b = a_enemy->GetPosition().y - (m * a_enemy->GetPosition().x);

			if (a_enemy->GetPosition().y > a_enemy->GetAttackExitPoint().y)
			{
				float x = a_enemy->GetPosition().x + a_enemy->GetSpeed() * timeDelta;
				float y = (m * x) + b;
				a_enemy->SetPosition(x, y);
			}
			else
			{
				a_enemy->attackExitChosen = false;
				a_enemy->SetAttackState(RETURN);
			}

			//TODO: turn bool attackExitIsChosen to false before leaving this state
			break;
		case RETURN:
			std::cout << "foo";
			break;
		}
	}

	//enemies attack logic
	/*
	enemies will attack from left / right most column depending on placement of group in relation to screen center (if too
	far to side, can't circle out) and after a certain time since last attack has elapsed. once column is selected, the top
	most enemy will circle out (clockwise if right side/ counter-clockwise if left)	and once y drops below lowest column the
	enemy selects an x on the opposite side of the player and then moves diagonally	across dropping bombs on regular time.
	If not hit, the enemy returns to screen at the same x and returns to column

	want to implement if time:
	enemy sprite rotating as circle is completed and points towards player position when firing.
	*/
	//which column? find enemy with lowest x and greatest x and which one of those is furthest from center
	/*Enemy* lowestX = nullptr;
	Enemy* highestX = nullptr;
	for (auto object : gameObjects)
	{
	if (dynamic_cast<Enemy*>(object) != 0)
	{
	Enemy* enemy = dynamic_cast<Enemy*>(object);
	if (lowestX == nullptr)
	{
	lowestX = enemy;
	}
	else if (highestX == nullptr)
	{
	highestX = enemy;
	}
	else
	{
	if (enemy->GetPosition().x < lowestX->GetPosition().x)
	{
	lowestX = enemy;
	}
	if (enemy->GetPosition().x > highestX->GetPosition().x)
	{
	highestX = enemy;
	}
	}
	}
	}*/
	//#ifdef _DEBUG
	//	std::cout << "low: " << lowestX->GetPosition().x << std::endl;
	//	std::cout << "high: " << highestX->GetPosition().x << std::endl;
	//	system("pause");
	//#endif




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

void GameState::Update(float a_timestep, StateMachine* a_SMPointer)
{
	for (auto object : gameObjects)
	{
		object->Update(a_timestep);
		if (dynamic_cast<Enemy*>(object) != 0)
		{
			EnemyLogic(dynamic_cast<Enemy*>(object), a_timestep);
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
	for (auto object : gameObjects)
	{
		object->Draw();
	}

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
	float enemyY = screenHeight *0.9f;

	for (int i = 0; i < NUM_ENEMYS; i++)
	{

		Enemy* enemy = new Enemy();
		enemy->SetSize(58, 26);
		enemy->SetSpeed(3.5f);

		enemy->SetSpriteId(CreateSprite("./images/invaders_1_00.png", enemy->GetWidth(), enemy->GetHeight(), true));

		//check if need new line of enemy
		if (enemyX > screenWidth * 0.8f)
		{
			enemyX = screenWidth * 0.2f;
			enemyY -= 0.04f * screenHeight;
		}

		//initialize position
		enemy->SetPosition(enemyX, enemyY);

		//increment next enemy's x position
		enemyX += 0.1f * screenWidth;

		enemy->SetScoreValue(30);


		gameObjects.push_back(enemy);
	}
}


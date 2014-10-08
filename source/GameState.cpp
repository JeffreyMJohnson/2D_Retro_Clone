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
	player->SetX(screenWidth * 0.5f);
	player->SetY(50.0f);
	player->SetSpeed(10.0f);

	////add player to dynamic array
	gameObjects.push_back(player);

	MoveSprite(player->GetSpriteID(), player->GetX(), player->GetY());

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

void GameState::EnemyLogic(Enemy* a_enemy, float timeDelta)
{


	//enemies moving left and right logic
	if (a_enemy->GetX() > screenWidth * 0.95f)
	{
		a_enemy->SetX(screenWidth * 0.95f);
		ReverseEnemies();
		//break;
	}
	else if (a_enemy->GetX() < screenWidth * 0.05f)
	{
		a_enemy->SetX(screenWidth * 0.05f);
		ReverseEnemies();
	}

	if (attackTimer >= 0)
	{
		attackTimer -= timeDelta;
	}
	else
	{
		attackTimer = 50.0f;
		a_enemy->isAttacking = true;

	}
	if (a_enemy->isAttacking)
	{
		if (attackAngle == 90.0f)
		{
			Point2d originalPos;
			originalPos.x = a_enemy->GetX();
			originalPos.y = a_enemy->GetY();
			a_enemy->SetOriginalPos(originalPos);
		}

		if (attackAngle <= 270.0f)
		{
			float x = (a_enemy->GetOriginalPosition().x + (a_enemy->GetAttackRadius() * cos(a_enemy->GetAttackAngle()) / a_enemy->GetAttackRadius()));
			float y = (a_enemy->GetOriginalPosition().y + (a_enemy->GetAttackRadius() * sin(a_enemy->GetAttackAngle()) / a_enemy->GetAttackRadius()));
			attackAngle += .01;

			a_enemy->SetX(x);
			a_enemy->SetY(y);
		}
		else
		{
			a_enemy->isAttacking = false;
			attackAngle = 90.0f;

		}
	}

	/*
	this is code to get x and y for circle!!!
	float x, y;
	if (angle <= 360)
	{
	x = xPos + (radius * cos(angle * 180 / PI));
	y = yPos + (radius * sin(angle * 180 / PI));
	angle += .0001f;

	cout << "xpos: " << x << endl;
	cout << "ypos: " << y << endl;
	//system("pause");
	}
	else
	{
	angle = 0.0f;
	x = 0;
	y = 0;
	}

	MoveSprite(myTextureHandle, x, y);

	*/

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
	if (enemy->GetX() < lowestX->GetX())
	{
	lowestX = enemy;
	}
	if (enemy->GetX() > highestX->GetX())
	{
	highestX = enemy;
	}
	}
	}
	}*/
	//#ifdef _DEBUG
	//	std::cout << "low: " << lowestX->GetX() << std::endl;
	//	std::cout << "high: " << highestX->GetX() << std::endl;
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
//				if (CheckCollision(a_player->bullets[i].x, a_player->bullets[i].y, enemyShip->GetX(), enemyShip->GetY(), 30.0f) &&
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
//	if (a_enemy->GetX() > screenWidth * 0.9f && !lowerAliens)
//	{
//		direction = -1;
//		lowerAliens = true;
//		//break;
//	}
//	else if (a_enemy->GetX() < screenWidth * 0.1f && !lowerAliens)
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

	if (enemy->GetY() <= (0.05f * screenHeight))
	{
	gameOver = true;
	gameOverTimer = 2;
	return;
	}

	enemy->SetY(enemy->GetY() - (0.05f * screenHeight));
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


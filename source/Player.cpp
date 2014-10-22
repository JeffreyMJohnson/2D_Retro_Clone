#include "Player.h"

Player::Player(char* filePath, float a_width, float a_height)
{
	/*width = 64;
	height = 32;*/

	width = a_width;
	height = a_height;
	spriteID = CreateSprite(filePath, a_width, a_height, true);
	////start position
	//SetPosition(0, 0);

	//shootKey = 32;//space key
	////currentReloadBulletTime = 0.0f;
	////maxBulletReloadTime = .25f;

	////AddScore(0);

}

//initialize player with position, velocity, radius (collider),health, speed and alive
void Player::Init(Point2d a_pos, Point2d a_velocity, float a_radius, int a_health)
{
	position = a_pos;
	velocity = a_velocity;
	collider.radius = a_radius;
	health = a_health;
	speed = 10.0f;
	alive = true;
	//bullet.Init(a_pos, Point2d{ 0, 1 }, 100.0f, 15.0f, 1);

}

//input handling
void Player::Input()
{
	//start with 0 velocity
	velocity = Point2d{ 0, 0 };

	if (IsKeyDown(GLFW_KEY_A))
	{
		if (position.x <= 0)
		{
			position.x = 0;
			velocity.x = 0;
		}
		else
		{
			velocity.x = -1;
		}

	}

	if (IsKeyDown(GLFW_KEY_D))
	{
		if (position.x >= screenWidth - (height * 0.5f))
		{
			position.x = screenWidth - (height * 0.5f);
			velocity.x = 0;
		}
		else
		{
			velocity.x = 1;
		}

	}

	if (IsKeyDown(GLFW_KEY_SPACE))
	{
		Shoot();
	}
}

//handle shooting
void Player::Shoot()
{
	BulletManager::playerBullet->velocity = Point2d{ 0, 1 };
	BulletManager::playerBullet->alive = true;
}

void Player::SetMovementKeys(unsigned int a_moveLeft, unsigned int a_moveRight)
{
	moveLeftKey = a_moveLeft;
	moveRightKey = a_moveRight;
}

void Player::SetMovementExtremes(unsigned int a_leftExtreme, unsigned int a_rightExtreme)
{
	leftMovementExtreme = a_leftExtreme;
	rightMovementExtreme = a_rightExtreme;
}

void Player::Update(float a_delta)
{
	Input();

	position.x += velocity.x * speed * a_delta;
	position.y += velocity.y * speed * a_delta;

	MoveSprite(spriteID, position.x, position.y);

	if (!BulletManager::playerBullet->alive)
	{
		//set bullet makes bullet alive but want it dead here until player shoots
		BulletManager::SetBullet(PLAYER, Point2d{ position.x, (position.y + 30) }, Point2d{ 0, 0 }, 100, 1);
		BulletManager::playerBullet->alive = false;
	}

}

void Player::Draw()
{
	//MoveSprite(spriteID, position.x, position.y);
	DrawSprite(spriteID);
}

void Player::SetMoveLeftKey(unsigned int a_moveKey)
{
	moveLeftKey = a_moveKey;
}

unsigned int Player::GetLeftMoveKey()
{
	return moveLeftKey;
}

void Player::SetMoveRightgKey(unsigned int a_moveKey)
{
	moveRightKey = a_moveKey;
}

unsigned int Player::GetMoveRightKey()
{
	return moveRightKey;
}

void Player::SetShootKey(unsigned int a_shootKey)
{
	shootKey = a_shootKey;
}

unsigned int Player::GetShootKey()
{
	return shootKey;
}

void Player::SetLeftMovementExtreme(unsigned int a_movementExtreme)
{
	leftMovementExtreme = a_movementExtreme;
}

unsigned int Player::GetLeftMovementExtreme()
{
	return leftMovementExtreme;
}

void Player::SetRightMovementExtreme(unsigned int a_movementExtreme)
{
	rightMovementExtreme = a_movementExtreme;
}

unsigned int Player::GetRightMovementExtreme()
{
	return rightMovementExtreme;
}

//int Player::GetScore()
//{
//	return score;
//}
//
//char * Player::GetScoreAsString()
//{
//	return scoreAsString;
//}

Player::~Player()
{
}
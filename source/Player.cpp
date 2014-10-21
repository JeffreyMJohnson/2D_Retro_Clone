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
	speed = 1.0f;
	alive = true;

}

//input handling
void Player::Input()
{
	//start with 0 velocity
	velocity = Point2d{ 0, 0 };

	if (IsKeyDown(GLFW_KEY_A))
	{
		if (position.y <= 0)
		{
			position.y = 0;
			velocity.y = 0;
		}
		else
		{
			velocity.y = -1;
		}

	}

	if (IsKeyDown(GLFW_KEY_D))
	{
		if (position.y >= screenWidth - (height * 0.5f))
		{
			position.y = screenWidth - (height * 0.5f);
			velocity.y = 0;
		}
		else
		{
			velocity.y = 1;
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
	//only get one bullet at a time so check if alive
	//if (!bullet.alive)
	//{
	//	bullet.alive = true;
	//	bullet.velocity = Point2d{ 0, 1 };
	//}
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

	/*if (IsKeyDown(moveLeftKey))
	{
	position.x -= a_delta * speed;
	if (position.x < (leftMovementExtreme + width * 0.5f))
	{
	position.x = (leftMovementExtreme + width * 0.5f);
	}
	}

	if (IsKeyDown(moveRightKey))
	{
	position.x += a_delta * speed;
	if (position.x >(rightMovementExtreme - width * 0.5f))
	{
	position.x = (rightMovementExtreme - width * 0.5f);
	}
	}*/
}

void Player::Draw()
{
	//MoveSprite(spriteID, position.x, position.y);
	DrawSprite(spriteID);
}

/*
Get inactive bullet from bullets array (if any) and initialize with player's current position.
*/
//void Player::Shoot(unsigned int a_textureID, float _a_delta)
//{
//	if (IsKeyDown(shootKey) && currentReloadBulletTime >= maxBulletReloadTime)
//	{
//		GetInactiveBullet().InitializeBullet(x, y, 0, 500, a_textureID);
//		currentReloadBulletTime = 0.0f;
//	}
//	currentReloadBulletTime += _a_delta;
//	
//}

/*
Returns reference to first Bullet object in bullets array that is not active. If all are active
returns the first bullet in the array.
*/
//Bullet& Player::GetInactiveBullet()
//{
//	for (int i = 0; i < MAX_BULLETS; i++)
//	{
//		if (!bullets[i].isActive)
//		{
//			return bullets[i];
//		}
//	}
//
//	return bullets[0];
//}




//Setters / getters

//void Player::SetSpeed(float a_speed)
//{
//	speed = a_speed;
//}
//
//float Player::GetSpeed()
//{
//	return speed;
//}

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
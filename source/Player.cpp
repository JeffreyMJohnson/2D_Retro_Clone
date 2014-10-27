#include "Player.h"

Player::Player(char* filePath, float a_width, float a_height)
{
	width = a_width;
	height = a_height;

	//sprites for shoot animation
	spriteID_A = CreateSprite(filePath, a_width, a_height, true);
	spriteId_B = CreateSprite("./images/player/galaxian_b2.png", 46.0f, 69.0f, true);
	spriteID = spriteID_A;
}

//initialize player with position, velocity, radius (collider),health, speed and alive
void Player::Init(Point2d a_pos, Point2d a_velocity, float a_radius, int a_health)
{
	position = a_pos;
	velocity = a_velocity;
	collider.radius = a_radius;
	health = a_health;

	speed = 50.0f;
	alive = true;

}

//input handling
void Player::Input()
{
	//start with 0 velocity
	velocity = Point2d();

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

	if (!BulletManager::playerBullet->alive && IsKeyDown(GLFW_KEY_SPACE))
	{
		Shoot();
	}
}

//handle shooting
void Player::Shoot()
{
	BulletManager::SetBullet(PLAYER, Point2d(position.x, position.y + 20.0f), Point2d(0, 1), 300.0f, 1);
}

void Player::Update(float a_delta)
{
	if (alive)
	{
		Input();

		position.x += velocity.x * speed * a_delta;
		position.y += velocity.y * speed * a_delta;

		MoveSprite(spriteID, position.x, position.y);

		collider.center = position;
	}
	else
	{
		//move to original position
		position = Point2d(screenWidth * 0.5f, 100.0f);
		MoveSprite(spriteID, position.x, position.y);
		collider.center = position;
	}

}

void Player::Draw()
{
	//MoveSprite(spriteID, position.x, position.y);
	if (alive)
	{
		//shoot animation
		if (BulletManager::playerBullet->alive)
		{
			spriteID = spriteId_B;
		}
		else
		{
			spriteID = spriteID_A;
		}
		DrawSprite(spriteID);
	}

}



Player::~Player()
{
}
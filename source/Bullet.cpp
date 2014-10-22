#include "Bullet.h"


//Bullet::Bullet()
//{
//	//refactored
//	//position.x = 0.0f;
//	//position.y = 0.0f;
//	////velocityX = 0;
//	//velocityY = 0;
//	//direction = 0;
//	//spriteID = 0;
//	//isActive = false;
//	//height = 15.0f;
//	//width = 4.0f;
//}

Bullet::Bullet(const char* filePath, float a_width, float a_height)
{
	width = a_width;
	height = a_height;
	spriteID = CreateSprite(filePath, a_width, a_height, true);
	alive = false;
	Init(Point2d{ 0, 0 }, Point2d{ 0, 0 }, 100, 15, 1);
}

void Bullet::Init(Point2d a_pos, Point2d a_velocity, float a_speed, float a_radius, int a_health)
{
	position = a_pos;
	velocity = a_velocity;
	speed = a_speed;
	collider.radius = a_radius;
	health = a_health;
}

//move this bullet to given position with velocity, speed and health
void Bullet::Spawn(Point2d a_pos, Point2d a_velocity, float a_speed, int a_health)
{
	position = a_pos;
	velocity = a_velocity;
	speed = a_speed;

	health = a_health;
	alive = true;
}

void Bullet::Update(float a_delta)
{

	position.x += velocity.x * speed * a_delta;
	position.y += velocity.y * speed * a_delta;
	collider.center = position;

	MoveSprite(spriteID, position.x, position.y);

	if (position.y > screenHeight + (height * 0.5f))
	{
		alive = false;
	}

	//x += velocityX * a_delta;

	//if (isActive)
	//{
	//	position.y += velocityY * a_delta * direction;
	//}


	//if (isActive && position.y > screenHeight + height * 0.5f)
	//{
	//	isActive = false;
	//}

}

void Bullet::Draw()
{
	//MoveSprite(spriteID, position.x, position.y);
	DrawSprite(spriteID);
}

//void Bullet::InitializeBullet(float a_x, float a_y, float a_velocityX, float a_velocity_y, unsigned int a_textureId)
//{
//	position.x = a_x;
//	position.y = a_y;
//	//velocityX = a_velocityX;
//	velocityY = a_velocity_y;
//	spriteID = a_textureId;
//
//	isActive = true;
//}
//
//bool Bullet::isCollided(Entity* enemy)
//{
//	float collisionDx = height * 0.5f;
//	enemy->SetCollisionDx(enemy->GetHeight() * 0.5f);
//	return pow(enemy->GetPosition().x - position.x, 2) + pow(enemy->GetPosition().y - position.y, 2) <= pow(collisionDx + enemy->GetCollisionDx(), 2);
//
//
//}

Bullet::~Bullet()
{
}
#include "Bullet.h"

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
}

void Bullet::Draw()
{
	if (alive)
	{
		DrawSprite(spriteID);
	}

}
Bullet::~Bullet()
{
}
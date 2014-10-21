#include "Bullet.h"


Bullet::Bullet()
{
	position.x = 0.0f;
	position.y = 0.0f;
	//velocityX = 0;
	velocityY = 0;
	direction = 0;
	spriteID = 0;
	isActive = false;
	height = 15.0f;
	width = 4.0f;
}

void Bullet::Update(float a_delta)
{
	//x += velocityX * a_delta;

	if (isActive)
	{
		position.y += velocityY * a_delta * direction;
	}


	if (isActive && position.y > screenHeight + height * 0.5f)
	{
		isActive = false;
	}

}

void Bullet::Draw()
{
	MoveSprite(spriteID, position.x, position.y);
	DrawSprite(spriteID);
}

void Bullet::InitializeBullet(float a_x, float a_y, float a_velocityX, float a_velocity_y, unsigned int a_textureId)
{
	position.x = a_x;
	position.y = a_y;
	//velocityX = a_velocityX;
	velocityY = a_velocity_y;
	spriteID = a_textureId;

	isActive = true;
}

bool Bullet::isCollided(Entity* enemy)
{
	float collisionDx = height * 0.5f;
	enemy->SetCollisionDx(enemy->GetHeight() * 0.5f);
	return pow(enemy->GetPosition().x - position.x, 2) + pow(enemy->GetPosition().y - position.y, 2) <= pow(collisionDx + enemy->GetCollisionDx(), 2);


}

Bullet::~Bullet()
{
}
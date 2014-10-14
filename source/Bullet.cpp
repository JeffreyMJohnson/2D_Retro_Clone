#include "Bullet.h"


Bullet::Bullet()
{
	x = 0;
	y = 0;
	//velocityX = 0;
	velocityY = 0;
	textureId = 0;
	isActive = false;
	height = 0.0f;
	width = 0.0f;
}

void Bullet::Update(float a_delta)
{
	//x += velocityX * a_delta;

	if (isActive)
	{
		y += velocityY * a_delta;
	}


	if (isActive && y > screenHeight + height * 0.5f)
	{
		isActive = false;
	}

}

void Bullet::Draw()
{
	MoveSprite(textureId, x, y);
	DrawSprite(textureId);
}

void Bullet::InitializeBullet(float a_x, float a_y, float a_velocityX, float a_velocity_y, unsigned int a_textureId)
{
	x = a_x;
	y = a_y;
	//velocityX = a_velocityX;
	velocityY = a_velocity_y;
	textureId = a_textureId;

	isActive = true;
}

bool Bullet::isCollided(Enemy* enemy)
{
	float collisionDx = height * 0.5f;
	enemy->SetCollisionDx(enemy->GetHeight() * 0.5f);
	return pow(enemy->GetPosition().x - x, 2) + pow(enemy->GetPosition().y - y, 2) <= pow(collisionDx + enemy->GetCollisionDx(), 2);


}

Bullet::~Bullet()
{
}
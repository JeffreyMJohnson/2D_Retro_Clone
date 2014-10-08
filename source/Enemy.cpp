#include "Enemy.h"
#include <iostream>
#include <math.h>

int Enemy::activeEnemyCount = 0;


Enemy::Enemy()
{
	isActive = true;
	direction = 1;
	activeEnemyCount++;
	isAttacking = false;
	originalPosition.x = 0;
	originalPosition.y = 0;
	attackAngle = 90.0f;
	attackRadius = 5.0f;
}

void Enemy::Update(float a_delta)
{

	//all fucked up!!
	if (isActive)
	{
		x += speed * direction * a_delta;
		////x += speed * direction * a_delta;
		//if (!isAttacking)
		//{
		//	attackStartPos.x = x;
		//	attackStartPos.y = y;
		//	isAttacking = true;
		//}
		//else
		//{
		//	if (y - attackStartPos.y < 50)
		//	{
		//		y += 10.0f * a_delta;
		//	}
		//	else
		//	{
		//		if (attackAngle < 50)
		//		{
		//			x += (50 * cos(attackAngle)) * a_delta;
		//			attackAngle++;
		//		}
		//		else
		//		{
		//			isAttacking = false;
		//			std::cout << "done\n";
		//		}

		//	}
		//}
	}
}

void Enemy::Attack(float timeDelta)
{
	//initalize data before starting
	/*if (!isAttacking)
	{
		attackStartPos.x = x;
		attackStartPos.y = y;
	}
	else
	{
		while (y - attackStartPos.y < 100)
		{
			y += speed * timeDelta;
		}
	}*/




}

void Enemy::setMovementExtremes(unsigned int a_leftExtreme, unsigned int a_rightExtreme)
{
	leftMovementExtreme = a_leftExtreme;
	rightMovementExtreme = a_rightExtreme;
}

////move the enemy 
//void Enemy::Move(float a_speed, int a_direction, float a_delta)
//{
//	//changed like lecture example
//	x += a_speed * a_direction * a_delta;
//
//
//}

//draw the enemy
void Enemy::Draw()
{
	if (isActive)
	{
		MoveSprite(spriteID, x, y);
		DrawSprite(spriteID);
	}

}

/*
check all collisions and apply effects:
IsCollidedLeftWall - flip speedX and add a speed to speedY else speedY = 0
IsCollideRighttWall - flip speedX and add a speed to speedY else speedY = 0
params:
a_speedY: Speed to set speedY variable to make enemy move along Y axis. Should
be negative so enemy moves 'down'.
*/
void Enemy::CheckCollisions()
{
	if (IsCollidedLeftWall() || IsCollidedRightWall())
	{
		speed *= -1;
		y -= height / 2;
		MoveSprite(spriteID, x, y);
	}
}

// SETTERS / GETTERS

void Enemy::SetDirection(int a_direction)
{
	direction = a_direction;
}

int Enemy::GetDirection()
{
	return direction;
}

void Enemy::SetSpeed(float a_speed)
{
	speed = a_speed;
}

float Enemy::GetSpeed()
{
	return speed;
}

void Enemy::SetLeftMoveExtreme(unsigned int a_leftExtreme)
{
	leftMovementExtreme = a_leftExtreme;
}

unsigned int Enemy::GetLeftMoveExtreme()
{
	return leftMovementExtreme;
}

void Enemy::SetRightMoveExtreme(unsigned int a_righttExtreme)
{
	rightMovementExtreme = a_righttExtreme;
}

unsigned int Enemy::GetRightMoveExtreme()
{
	return rightMovementExtreme;
}

void Enemy::SetScoreValue(int a_scoreValue)
{
	scoreValue = a_scoreValue;
}

int Enemy::GetScoreValue()
{
	return scoreValue;
}

void Enemy::SetIsActive(bool a_isActive)
{
	isActive = a_isActive;
}

bool Enemy::GetIsActive()
{
	return isActive;
}

void Enemy::SetAttackAngle(float a_angle)
{
	attackAngle = a_angle;
}

float Enemy::GetAttackAngle()
{
	return attackAngle;
}

void Enemy::SetAttackRadius(float a_radius)
{
	attackRadius = a_radius;
}

float Enemy::GetAttackRadius()
{
	return attackRadius;
}

void Enemy::SetOriginalPos(Point2d a_point)
{
	originalPosition = a_point;
}

Point2d Enemy::GetOriginalPosition()
{
	return originalPosition;

}


Enemy::~Enemy()
{
}


//PRIVATE FUNCTIONS

/*
check if collided with right extreme (wall) and return true, otherwise return false
*/
bool Enemy::IsCollidedRightWall()
{
	if (x >= rightMovementExtreme)
	{
		x = rightMovementExtreme;
		return true;
	}
	return false;
}

/*
check if collided with left extreme (wall) and return true, otherwise return false
*/
bool Enemy::IsCollidedLeftWall()
{
	if (x <= leftMovementExtreme)
	{
		x = leftMovementExtreme;
		return true;
	}
	return false;
}

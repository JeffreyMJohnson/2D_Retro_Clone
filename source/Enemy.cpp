#include "Enemy.h"
#include <iostream>


int Enemy::activeEnemyCount = 0;


Enemy::Enemy()
{
	isActive = true;
	direction = 1;
	activeEnemyCount++;
	isAttacking = false;
	isLeader = false;
	returnPosition.x = 0;
	returnPosition.y = 0;
	//in radians so convert;
	attackAngle = DegreeToRadians(90.0f);
	attackRadius = 50.0f;
	attackState = MOVE;
	attackExitPoint = Point2d{ 0, 0 };
	attackExitChosen = false;
	attackSlope = 0.0f;
	attackYIntercept = 0.0f;
	attackDirection = 0;
	attackSpeed = 5.0f;
	shootMaxTime = .5f;
	shootTimer = shootMaxTime;

	player = nullptr;

	//LoadBullets();

	//reset in GameState.cpp enemyLogic() function
	//attackTimer = 5.0f;

}
void Enemy::Update(float a_delta)
{
	if (isActive && attackState != ATTACK)
	{
		position.x += speed * direction * a_delta;
	}
	if (isAttacking)
	{
		returnPosition.x += speed * direction * a_delta;
		//SetSpriteColour(spriteID, SColour(255, 0, 0, 255));
		Attack(a_delta);

		//if (isActive && attackState != ATTACK)
		//{
		//	position.x += speed * direction * a_delta;
		//}
		//if (isAttacking)
		//{
		//	returnPosition.x += speed * direction * a_delta;
		//	//SetSpriteColour(spriteID, SColour(255, 0, 0, 255));
		//	Attack(a_delta);
		//}

		//update bullets
		if (attackState == ATTACK)
		{

			//for (auto bullet : *enemyBullets)
			//{
			//	if (bullet->isActive)
			//	{
			//		bullet->Update(a_delta);
			//	}
			//}
		}
	}
}

void Enemy::SetEnemyBullets(std::vector<Bullet*>* a_enemyBullets)
{
	enemyBullets = a_enemyBullets;
}

/*
in order to go negative direction when the circle reaches 0 degrees it will reset to 360 and fail existing test.
solution is an equality test for 270 degrees, but with float math will need to use epsilon - (fabs(result - expectedResult) > epsilon)
*/
void Enemy::Attack(float timeDelta)
{
	float epsilon = .01;

	switch (attackState)
	{
	case WAIT:
		std::cout << "waiting....\n";
		break;
	case MOVE:
		if (position.y < returnPosition.y + attackRadius)
		{
			position.y += attackSpeed * timeDelta;
		}
		else
		{
			attackState = CIRCLE;
		}
		break;
	case CIRCLE:
		/*points gotten by using parametric circle equation - given the center point of a circle a,b and the diameter d
		the points on the circumference given an angle from a previous point on circle theta:
		x = a + d * cos(theta)
		y = a + d * sin(theta)
		*/
		//attackAngle is in radians convert to be in degrees
		//if (attackAngle <= DegreeToRadians(270.0f))
		if (fabs((attackAngle - DegreeToRadians(270.0f))) > epsilon)
		{
			float x = (returnPosition.x + (attackRadius * cos(attackAngle)));
			float y = (returnPosition.y + (attackRadius * sin(attackAngle)));
			/*float cosA = cos(attackAngle);
			float sinA = sin(attackAngle);
			float angleR = attackAngle;
			float angleD = RadiansToDegrees(attackAngle);*/
			attackAngle = attackAngle + DegreeToRadians(.05f) * attackDirection * attackSpeed;
			if (RadiansToDegrees(attackAngle) <= 0)
			{
				attackAngle = DegreeToRadians(360.0f);
			}
			//attackAngle = attackAngle * attackDirection + DegreeToRadians(attackSpeed * timeDelta);

			SetPosition(x, y);
		}
		else
		{
			attackAngle = 90.0f;
			attackState = ATTACK;
		}
		//		break;
		break;
	case ATTACK:
		assert(player != nullptr && player != 0);


		if (!attackExitChosen)
		{
			if (position.x < player->GetPosition().x)
			{
				//pick to right of player
				attackDirection = 1;
			}
			else//enemy to right or equal of player so go left
			{
				attackDirection = -1;
			}
			Point2d exit{ 0, 0 };
			exit.x = player->GetPosition().x + 100.0f * attackDirection;
			attackExitPoint = exit;
			attackExitChosen = true;

			//slope formula -> m = (y1 -y2) / (x1 - x2)
			float m = GetSlopeOfLine(position, attackExitPoint);
			attackSlope = m;

			//point-slope formula -> y - y1 = m * (x - x1) choose any point on line as (x1, y1)
			//b = y - m * x
			//y = m * x + b
			float b = position.y - (m * position.x);
			attackYIntercept = b;
		}

		if (shootTimer <= 0)
		{
			std::cout << "shoot\n";
			/*Bullet* b = GetInactiveBullet();
			b->SetPosition(position.x, position.y - b->GetHeight() * 0.5f);*/
			//b->isActive = true;
			shootTimer = shootMaxTime;

		}
		else
		{
			shootTimer -= timeDelta;
		}
		//increment x
		if (position.y > attackExitPoint.y && position.x < attackExitPoint.x)
		{
			float x = position.x + attackSpeed * timeDelta;
			float y = (attackSlope * x) + attackYIntercept;
			position = Point2d{ x, y };
		}
		//decrement x
		else if (position.y > attackExitPoint.y && position.x > attackExitPoint.x)
		{
			float x = position.x - attackSpeed * timeDelta;
			float y = (attackSlope * x) + attackYIntercept;
			position = Point2d{ x, y };
		}
		else
		{
			attackExitChosen = false;

			//set enemy x to original position and y to screenheight
			position = Point2d{ returnPosition.x, screenHeight };
			attackState = RETURN;

		}




		//if (!enemy->attackExitChosen && enemy->GetPosition().x < player->GetPosition().x)
		//		{
		//			//pick to the right of the player 
		//			Point2d exit{ 0, 0 };
		//			exit.x = player->GetPosition().x + 100.0f;
		//			enemy->SetAttackExitPoint(exit);
		//			enemy->attackExitChosen = true;
		//		}
		//		else if (!enemy->attackExitChosen) //enemy to right or equal of player so go left
		//		{
		//			Point2d exit{ 0, 0 };
		//			exit.x = player->GetPosition().x - 100.0f;
		//			enemy->SetAttackExitPoint(exit);
		//			enemy->attackExitChosen = true;
		//		}

		//		//TODO: get this refactored so only call once per attack sequence
		//		//slope formula - m = (y1 -y2) / (x1 - x2)
		//		m = GetSlopeOfLine(enemy->GetPosition(), enemy->GetAttackExitPoint());

		//		//TODO: get this refactored so only call once per attack sequence
		//		//point-slope formula - y - y1 = m * (x - x1) choose any point on line as (x1, y1)
		//		//b = y - m * x
		//		//y = m * x + b
		//		b = enemy->GetPosition().y - (m * enemy->GetPosition().x);

		//		//increment x
		//		if (enemy->GetPosition().y > enemy->GetAttackExitPoint().y && enemy->GetPosition().x < enemy->GetAttackExitPoint().x)
		//		{
		//			float x = enemy->GetPosition().x + enemy->GetSpeed() * timeDelta;
		//			float y = (m * x) + b;
		//			enemy->SetPosition(x, y);
		//		}
		//		//decrement x
		//		else if (enemy->GetPosition().y > enemy->GetAttackExitPoint().y && enemy->GetPosition().x > enemy->GetAttackExitPoint().x)
		//		{
		//			float x = enemy->GetPosition().x - enemy->GetSpeed() * timeDelta;
		//			float y = (m * x) + b;
		//			enemy->SetPosition(x, y);
		//		}
		//		else
		//		{
		//			enemy->attackExitChosen = false;

		//			//set enemy x to original position and y to screenheight
		//			enemy->SetPosition(enemy->GetreturnPosition().x, screenHeight);
		//			enemy->SetAttackState(RETURN);
		//		}


		break;
	case RETURN:
		if (position.y > returnPosition.y)
		{
			float y = position.y;
			float returnY = returnPosition.y;
			position = Point2d{ position.x, position.y - attackSpeed * timeDelta };
		}
		else
		{
			isAttacking = false;
			attackState = MOVE;
			std::cout << "done";
		}



		break;
	}


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

//Bullet* Enemy::GetInactiveBullet()
//{
//	for (Bullet* a_bullet : *enemyBullets)
//	{
//		if (!a_bullet->isActive)
//			return a_bullet;
//	}
//	return enemyBullets->front();
//}

float Enemy::GetSlopeOfLine(Point2d point1, Point2d point2)
{
	return (point1.y - point2.y) / (point1.x - point2.x);

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
		MoveSprite(spriteID, position.x, position.y);
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
		position.y -= height / 2;
		MoveSprite(spriteID, position.x, position.y);
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
	returnPosition = a_point;
}

Point2d Enemy::GetreturnPosition()
{
	return returnPosition;

}

void Enemy::SetAttackState(attackStates a_state)
{
	attackState = a_state;
}

attackStates Enemy::GetAttackState()
{
	return attackState;
}

void Enemy::SetAttackExitPoint(Point2d a_point)
{
	attackExitPoint = a_point;
}

Point2d Enemy::GetAttackExitPoint()
{
	return attackExitPoint;
}

void Enemy::SetReturnPosition(Point2d point)
{
	returnPosition = point;
}
Point2d Enemy::GetReturnPosition()
{
	return returnPosition;
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
	if (position.x >= rightMovementExtreme)
	{
		position.x = rightMovementExtreme;
		return true;
	}
	return false;
}

/*
check if collided with left extreme (wall) and return true, otherwise return false
*/
bool Enemy::IsCollidedLeftWall()
{
	if (position.x <= leftMovementExtreme)
	{
		position.x = leftMovementExtreme;
		return true;
	}
	return false;
}

float Enemy::DegreeToRadians(float angleInDegrees)
{
	return angleInDegrees * (PI / 180);
}

float Enemy::RadiansToDegrees(float angleInRadians)
{
	return angleInRadians * (180 / PI);
}
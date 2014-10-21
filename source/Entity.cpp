#include "Entity.h"


Entity::Entity()
{
	//collisionDx = 0.0f;
}

/*
return true if given other parameter position is within distance of this Entity position
within both object's set collision distance. otherwise will return false.
//make sure you set the collisionDx variable in each object before calling this function;
uses circle collider algorithm -> the collision formula is (x2 - x1)^2 + (y2 - y1)^2 <= (r1 + r2)^2
*/
//bool Entity::isCollided(Entity* other)
//{
//	using namespace std;
//	return pow(other->position.x - position.x, 2) + pow(other->position.y - position.y, 2) <= pow(collisionDx + other->collisionDx, 2);
//
//}

//void Entity::SetCollisionDx(float dx)
//{
//	collisionDx = dx;
//}
//
//float Entity::GetCollisionDx()
//{
//	return collisionDx;
//}

void Entity::SetSize(float a_width, float a_height)
{
	width = a_width;
	height = a_height;
}

void Entity::SetPosition(float a_x, float a_y)
{
	position.x = a_x;
	position.y = a_y;
}

void Entity::SetX(float x)
{
	position.x = x;
}

void Entity::SetY(float y)
{
	position.y = y;
}

Point2d Entity::GetPosition()
{
	return position;
}

void Entity::SetSpriteId(unsigned int a_spriteID)
{
	spriteID = a_spriteID;
}

unsigned int Entity::GetSpriteID()
{
	return spriteID;
}

void Entity::SetWidth(float a_width)
{
	width = a_width;
}

float Entity::GetWidth()
{
	return width;
}

void Entity::SetHeight(float a_height)
{
	height = a_height;
}

float Entity::GetHeight()
{
	return height;
}

Entity::~Entity()
{
}

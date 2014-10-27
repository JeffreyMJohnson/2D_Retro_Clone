#include "Entity.h"


Entity::Entity()
{
}

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

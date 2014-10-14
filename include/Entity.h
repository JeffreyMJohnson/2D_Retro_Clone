#pragma once
#ifndef _ENTITY_H_
#include <math.h>

struct Point2d
{
	float x, y;
};

class Entity
{
public:
	Entity();

	void SetSize(float a_width, float a_height);
	
	void SetPosition(float a_x, float a_y);
	void SetX(float x);
	void SetY(float y);
	Point2d GetPosition();

	virtual void Update(float a_delta) = 0;
	virtual void Draw() = 0;

	void SetSpriteId(unsigned int a_spriteID);
	unsigned int GetSpriteID();

	void SetWidth(float a_width);
	float GetWidth();

	void SetHeight(float a_height);
	float GetHeight();

	void SetCollisionDx(float dx);
	float GetCollisionDx();

	bool isCollided(Entity* other);

	~Entity();

protected:
	unsigned int spriteID;
	float width;
	float height;
	Point2d position;
	float collisionDx;
};
#endif // !_ENTITY_H_

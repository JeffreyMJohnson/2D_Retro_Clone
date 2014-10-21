#pragma once
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "Utility.h"
#include "CircleCollider.h"

extern const int screenWidth;
extern const int screenHeight;

class Entity
{
public:



	unsigned int spriteID;
	float width;
	float height;
	Point2d position;
	Point2d velocity;
	float speed;
	bool alive;

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


	//refactored out to CircleCollider class
	/*void SetCollisionDx(float dx);
	float GetCollisionDx();
	bool isCollided(Entity* other);
	*/

	CircleCollider collider;


	~Entity();

protected:

	//float collisionDx;
};
#endif // !_ENTITY_H_

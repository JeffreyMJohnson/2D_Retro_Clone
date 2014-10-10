#pragma once
#ifndef _ENTITY_H_

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

	

	~Entity();

protected:
	unsigned int spriteID;
	float width;
	float height;
	Point2d position;
};
#endif // !_ENTITY_H_

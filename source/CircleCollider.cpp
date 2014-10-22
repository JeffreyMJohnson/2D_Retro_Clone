#include "CircleCollider.h"


CircleCollider::CircleCollider()
{
	radius = 0;
	center = Point2d{ 0, 0 };
}


CircleCollider::~CircleCollider()
{
}

/*
return true if given other parameter position is within distance of this Entity position
within both object's set collision distance. otherwise will return false.
//make sure you set the collisionDx variable in each object before calling this function;
uses circle collider algorithm -> the collision formula is (x2 - x1)^2 + (y2 - y1)^2 <= (r1 + r2)^2
*/
bool CircleCollider::isCollided(CircleCollider &other)
{
	using namespace std;

	return pow(other.center.x - center.x, 2) + pow(other.center.y - center.y, 2) <= pow(radius + other.radius, 2);

}

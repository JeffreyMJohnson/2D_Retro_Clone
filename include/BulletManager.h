#ifndef _BULLET_MANAGER_H_
#define _BULLET_MANAGER_H_
#include <vector>
#include "Bullet.h"
class BulletManager
{
public:
	BulletManager();
	~BulletManager();

	static void Init()
	{
		for (int i = 0; i < 100; i++)
		{
			bullets.emplace_back(new Bullet("./images/bullet.png", 3, 20));
		}
	}

	static bool SetBullet(TYPE type, Point2d a_pos, Point2d a_velocity, float a_speed, int a_health)
	{
		for (Bullet* bullet : bullets)
		{
			if (!bullet->alive)
			{
				bullet->Spawn(a_pos, a_velocity, a_speed, a_health);
				return true;
			}
		}
		return false;
	}

	static void Update(float a_delta)
	{
		for (Bullet* bullet : bullets)
		{
			if (bullet->alive)
			{
				bullet->Update(a_delta);
				bullet->Draw();
			}
		}
	}

private:
	static std::vector<Bullet*> bullets;
};

#endif
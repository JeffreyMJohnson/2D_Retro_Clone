#ifndef _BULLET_MANAGER_H_
#define _BULLET_MANAGER_H_
#include <vector>
#include "Bullet.h"
class BulletManager
{
public:
	BulletManager();


	static Bullet* playerBullet;
	static std::vector<Bullet*> enemyBullets;

	static void Init()
	{
		for (int i = 0; i < 100; i++)
		{
			enemyBullets.emplace_back(new Bullet("./images/bullet.png", 3, 20));
		}

		playerBullet = new Bullet("./images/bullet.png", 3, 15);
		playerBullet->Init(Point2d(), Point2d(0, 1), 0, 1, 1);
	}

	static bool SetBullet(TYPE type, Point2d a_pos, Point2d a_velocity, float a_speed, int a_health)
	{

		if (type == ENEMY)
		{
			for (Bullet* bullet : enemyBullets)
			{
				if (!bullet->alive)
				{
					bullet->Spawn(a_pos, a_velocity, a_speed, a_health);
					return true;
				}
			}
		}
		else
		{
			if (!playerBullet->alive)
			{
				playerBullet->Spawn(a_pos, a_velocity, a_speed, a_health);
				return true;
			}
		}
		return false;
	}

	static void Update(float a_delta)
	{

		playerBullet->Update(a_delta);
		playerBullet->Draw();




		for (Bullet* bullet : enemyBullets)
		{
			//need to recycle bullets off screen
			if (bullet->alive)
			{
				if (bullet->position.y > screenHeight + bullet->height * 0.5f){
					bullet->alive = false;
				}
				if (bullet->position.y < bullet->height * 0.5f)
				{
					bullet->alive = false;
				}
			}
			if (bullet->alive)
			{
				bullet->Update(a_delta);
				bullet->Draw();
			}
		}

	}

	~BulletManager()
	{
		for (int i = 0; i < 100; i++)
		{
			delete enemyBullets[i];
		}
		delete playerBullet;
	}

private:
	

};

#endif
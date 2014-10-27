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

		if (enemyBullets.size() == 0)
		{
			for (int i = 0; i < 100; i++)
			{
				Bullet* enemyBullet = new Bullet("./images/bullet.png", 5, 15);
				enemyBullet->Init(Point2d(), Point2d(0, -1), 10.0f, 1, 1);
				enemyBullets.emplace_back(enemyBullet);
				//enemyBullets.emplace_back(new Bullet("./images/bullet.png", 5, 15));
			}

			playerBullet = new Bullet("./images/bullet.png", 5, 15);
			playerBullet->Init(Point2d(), Point2d(0, 1), 0, 1, 1);
		}
		else
		{
			BulletManager::AllDead();
		}
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

	/*
	call this to set all bullets to not alive*/
	static void AllDead()
	{
		for (Bullet* bullet : enemyBullets)
		{
			bullet->alive = false;
		}
		playerBullet->alive = false;
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
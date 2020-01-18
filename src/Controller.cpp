#include "Controller.h"
#include <cmath>
#include <iostream>

namespace mrsd
{
	void Controller::control(const mrsd::Game& g, float t)
	{
		determineSafeSpots(g);
	}

	void Controller::createPlayer(Game& g)
	{
		if(p == 0)
		{
			p = new Player();
			p->dead = true;
		}
		if(p->dead)
		{
			p->dead = false;
			p->x = g.getWidth()/2;
			g.newPlayer(p);
		}
	}

	Prediction Controller::trackProjectile(const Projectile& p, const Game& g)
	{
		Prediction pred;

		float a, b, c, timeOfFlight=0, crashTime = g.getGameTime();
		a = -g.getGravity()/2;
		b = -p.vy;
		c = -p.y;
		if (p.vy > 0)
		{
			timeOfFlight += 2*p.vy/(-g.getGravity());	
			b = p.vy;
		}

		timeOfFlight += (-b + std::sqrt(b*b - 4*a*c))/(2*a)>0 ? (-b + std::sqrt(b*b - 4*a*c))/(2*a) : (-b - std::sqrt(b*b - 4*a*c))/(2*a);
		crashTime += timeOfFlight;

		pred.t = crashTime;
		pred.x = p.x + p.vx*timeOfFlight;

		// std::cout << g.getGameTime() << " y:" << p.y << " x: "<< p.x << "|"<< pred.t << "|" << pred.x << std::endl;

		return pred;
	}

	void Controller::determineSafeSpots(const Game& g)
	{
		if (!p or p->dead) return;
		currentTime = g.getGameTime();
		int width = g.getWidth();
		int state[width] = {0};
		std::list<Projectile> curProjectiles = g.getProjectiles();
		for(auto projectile: curProjectiles)
		{
			float explosionPoint = trackProjectile(projectile, g).x;
			for(int i = std::floor(explosionPoint - g.explosionSize -1);
					i <= std::ceil(explosionPoint + g.explosionSize + 1);
					++i)
				
				if (i>0 and i<width)
				{	state[i] = 1;
					if (i == int(p->x))
						{
							if (i<(width/2)) p->x = explosionPoint + g.explosionSize + 2;
							else p->x = explosionPoint - g.explosionSize - 2;
						}			
				}
		}

		 // Helper to display safeSpot grid with player
		// for (int i=0; i<200; ++i)
		// {
		// 	if(i == int(p->x)) std::cout << "-1";
		// 	else std::cout << state[i];
		// }
		// std::cout << std::endl;
	}

	int Controller::pickSafeSpot(const Game& g)
	{		
		return 0;
	}

}

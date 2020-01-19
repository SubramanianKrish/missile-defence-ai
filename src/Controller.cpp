#include "Controller.h"
#include <cmath>
#include <iostream>

namespace mrsd
{
	void Controller::control(const mrsd::Game& g, float t)
	{	
		float maxMovableDist = g.playerSpeed;
		float safeSpot = determineSafeSpots(g);
		// If player not created, or player dead do nothing in controller.
		if (safeSpot == -1) return;
		// If player not in danger at current position, do nothing.
		// std::cout << "Dist to safe spot: " << std::abs(p->x - safeSpot) << std::endl;
		if ((std::abs(p->x - safeSpot)) < maxMovableDist) return;
		// If player to left of safe spot, move right else move left by max dist.
		if (p->x < safeSpot)
		{
			for (auto explosion: g.getExplosions())
			{
				std::cout << explosion.x << " | ";
				if(std::abs(explosion.x - (p->x + maxMovableDist)) < (g.explosionSize+1)) return;
			}
			p->x += maxMovableDist;
		}
		else
		{
			for (auto explosion: g.getExplosions())
			{
				std::cout << explosion.x << " | ";
				if(std::abs(explosion.x - (p->x - maxMovableDist)) < (g.explosionSize+1)) return;
			}
			p->x -= maxMovableDist;
		}
		// std::cout << "Trying to go to: "<< safeSpot << std::endl;
		// std::cout << "current agent position: " << p->x << std::endl;
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

	float Controller::determineSafeSpots(const Game& g)
	{
		if (!p or p->dead) return -1;
		currentTime = g.getGameTime();
		int width = g.getWidth();
		// int state[width] = {0};
		std::list<Projectile> curProjectiles = g.getProjectiles();
		for(auto projectile: curProjectiles)
		{	
			float explosionPoint = trackProjectile(projectile, g).x;
			// std::cout << explosionPoint << " | ";
			if (abs(explosionPoint - p->x) <= g.explosionSize)
			{
				// std::cout << "In danger!!" << std::endl;
				// std::cout << "Projectile landing at" << explosionPoint << std::endl;
				// std::cout << "Agent at: " << p->x << std::endl;
				if(explosionPoint <= width/2)
				{
					// std::cout << "Target to move to" << explosionPoint + g.explosionSize + 2 << std::endl;
					return explosionPoint + g.explosionSize + 2;
				}	
				else
				{
					// std::cout << "Target to move to" << explosionPoint - g.explosionSize - 2 << std::endl;
					return explosionPoint - g.explosionSize - 2;
				} 
			}
		}
		return p->x;

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

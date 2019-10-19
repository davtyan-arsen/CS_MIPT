#include "particle.h"
#include <math.h>

MaterialPoint::MaterialPoint() { };
MaterialPoint::~MaterialPoint() { };

void MaterialPoint::move(float dt)
{
	pos += dt * vel;
}

Particle::Particle(sf::Vector2f& pos0, sf::Vector2f& vel0, float& mass0)
{
	pos = pos0;
	vel = vel0;
	mass = mass0;
	rad = mass / density;

	obj.setRadius(rad);
	obj.setOrigin(sf::Vector2f(rad, rad));
	obj.setFillColor(sf::Color::White);
	obj.setPosition(pos.x, pos.y);
}

Particle::~Particle() { };

void Particle::move(float dt)
{
	pos += dt * vel;
	obj.setPosition(pos);
}

void Particle::checkBorders(const int maxX, const int maxY)
{
	if (pos.x < rad || pos.x > maxX - rad)
		vel.x = -vel.x;
	if (pos.y < rad || pos.y > maxY - rad)
		vel.y = -vel.y;
}

void Particle::checkCollision(std::vector<Particle>& particles)
{
	for (auto& it : particles)
	{
		sf::Vector2f d = {it.pos.x - pos.x, it.pos.y - pos.y};
		float distance = sqrt( (pos.x - it.pos.x) * (pos.x - it.pos.x) + (pos.y - it.pos.y) * (pos.y - it.pos.y) );
		sf::Vector2f dv = { it.vel.x - vel.x, it.vel.y - vel.y };

		if (distance > 0 && ( distance <= rad + it.rad ) && ( dv.x * d.x + dv.y * d.y ) <= 0)
		{
			vel += 2 * it.mass / (mass + it.mass) * (dv.x * d.x + dv.y * d.y) / (distance * distance) * d;
			it.vel -= 2 * mass / (mass + it.mass) * (dv.x * d.x + dv.y * d.y) / (distance * distance) * d;
		}
	}
}
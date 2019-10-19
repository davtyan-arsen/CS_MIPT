#pragma once
#include <SFML\Graphics.hpp>

const float density = 0.05f;
const float maxV = 0.15f;
const int randMass = 3;
const float minMass = 1;

class MaterialPoint
{
public:
	MaterialPoint();

	virtual ~MaterialPoint();
	virtual void move(float);

	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Vector2f acc;
	float mass;
};

class Particle: public MaterialPoint
{
public:
	Particle(sf::Vector2f&, sf::Vector2f&, float&);
	~Particle();
	void move(float);
	void checkBorders(const int, const int);
	void checkCollision(std::vector<Particle>&);
	float rad;

	sf::CircleShape obj;
};
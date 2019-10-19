#include <cmath>
#include <SFML\Graphics.hpp>
#include "charge.h"

ElectricCharge::ElectricCharge(int chargeValue, int posX, int posY)
{
	charge = chargeValue;
	x = posX;
	y = posY;

	obj.setRadius(chargeRadius);
	obj.setPosition(x - chargeRadius, y - chargeRadius);
	if (charge > 0)
		obj.setFillColor(sf::Color::Red);
	else obj.setFillColor(sf::Color::Blue);
}

sf::Vector2f getTension(sf::Vector2f& cur, std::vector<ElectricCharge>& charges)
{
	sf::Vector2f E;
	float distance;
	for (auto it : charges)
	{
		distance = sqrt((cur.x - it.x) * (cur.x - it.x) + (cur.y - it.y) * (cur.y - it.y));
		E.x += it.charge * (cur.x - it.x) / pow(distance, 3);
		E.y += it.charge * (cur.y - it.y) / pow(distance, 3);
	}

	float absE = sqrt(E.x * E.x + E.y * E.y);
	E.x = smoothFactor * E.x / absE;
	E.y = smoothFactor * E.y / absE;

	return E;
}
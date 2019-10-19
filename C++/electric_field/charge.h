#pragma once

const int chargeRadius = 10;
const int lineLength = 2000;
const float smoothFactor = 5;

class ElectricCharge
{
public:
	ElectricCharge(int, int, int);
	~ElectricCharge() {};
	sf::CircleShape obj;
	int charge;
	int x;
	int y;
};

sf::Vector2f getTension(sf::Vector2f&, std::vector<ElectricCharge>&);
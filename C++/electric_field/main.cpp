#include <iostream>
#include <SFML\Graphics.hpp>

#include "charge.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 900), "My Window");
	sf::Event event;
	static bool isPressed = false;

	std::vector<ElectricCharge> charges;
	std::vector<sf::VertexArray> lines;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!isPressed) 
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					ElectricCharge temp = {1, mousePos.x, mousePos.y};
					charges.push_back(temp);

					isPressed = true;
					break;
				}
			}
			else isPressed = false;

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				if (!isPressed)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					ElectricCharge temp = { -1, mousePos.x, mousePos.y };
					charges.push_back(temp);

					isPressed = true;
					break;
				}
			}
			else isPressed = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				if (!isPressed)
				{
					sf::VertexArray linePlus(sf::LinesStrip, lineLength);
					sf::VertexArray lineMinus(sf::LinesStrip, lineLength);

					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f curPlus = sf::Vector2f(mousePos.x, mousePos.y);
					sf::Vector2f curMinus = sf::Vector2f(mousePos.x, mousePos.y);

					for (int i = 0; i < lineLength; i++)
					{
						linePlus[i].position = sf::Vector2f(curPlus.x, curPlus.y);
						lineMinus[i].position = sf::Vector2f(curMinus.x, curMinus.y);
						sf::Vector2f Eplus = getTension(curPlus, charges);
						sf::Vector2f Eminus = getTension(curMinus, charges);
						curPlus += Eplus;
						curMinus -= Eminus;
					}

					lines.push_back(linePlus);
					lines.push_back(lineMinus);

					isPressed = true;
					break;
				}
				else isPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			{
				charges.clear();
				lines.clear();
			}
		}

		for (auto it : charges)
			window.draw(it.obj);

		for (auto it : lines)
			window.draw(it);

		window.display();
		window.clear();
	}
	
	return 0;
}
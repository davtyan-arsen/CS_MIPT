#include <iostream>
#include <SFML\Graphics.hpp>
#include "particle.h"

//draw(window, particles)

static const int windowResX = 1600;
static const int windowResY = 900;

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowResX, windowResY), "My Window");
	sf::Event event;
	sf::Clock clock;
	static bool isPressed = false;

	std::vector<Particle> particles;

	while (window.isOpen())
	{
		sf::Time time = clock.getElapsedTime();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!isPressed)
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f pos = {(float)mousePos.x, (float)mousePos.y};
					float vx = maxV * sin(std::rand());
					float vy = maxV * sin(std::rand());
					sf::Vector2f vel = { vx, vy };
					float mass = rand() % randMass + minMass;
					Particle temp = { pos, vel, mass };
					particles.push_back(temp);

					isPressed = true;
					break;
				}
			}
			else isPressed = false;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				particles.clear();
		}
		
		for (auto& it : particles)
		{
			it.checkBorders(windowResX, windowResY);
			it.checkCollision(particles);
			it.move(time.asSeconds());
			window.draw(it.obj);
		}

		window.display();
		window.clear();
	}
	
	return 0;
}
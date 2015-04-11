#pragma once
#include <SFML/Graphics.hpp>

class Collector
{
private:
	sf::CircleShape shape;

public:
	Collector(const float mRadius);
	virtual ~Collector(void) = default;

	void update(sf::Vector2f mousePosition);
	void draw(sf::RenderTarget& target);
};
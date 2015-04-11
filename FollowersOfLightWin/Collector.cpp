#include "Collector.h"

Collector::Collector()
{
	shape.setFillColor(sf::Color(0, 0, 0, 25));
	shape.setOutlineThickness(2);
	shape.setOutlineColor(sf::Color(70, 70, 150, 55));
	shape.setPointCount(150);
}

void Collector::setRadius(float mRadius)
{
	shape.setRadius(mRadius);
	shape.setOrigin(mRadius, mRadius);
}

void Collector::update(const sf::RenderWindow& window)
{
	sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);
	shape.setPosition(mousePosition);
}

void Collector::draw(sf::RenderTarget& target)
{
	target.draw(shape);
}

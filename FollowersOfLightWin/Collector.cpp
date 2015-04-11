#include "Collector.h"

Collector::Collector(const float mRadius)
{
	shape.setRadius(mRadius);
	shape.setOrigin(mRadius, mRadius);
	shape.setFillColor(sf::Color(0, 0, 0, 25));
	shape.setOutlineThickness(2);
	shape.setOutlineColor(sf::Color(70, 70, 150, 55));
	shape.setPointCount(150);
}

void Collector::update(sf::Vector2f mousePosition)
{
	shape.setPosition(mousePosition);
}

void Collector::draw(sf::RenderTarget& target)
{
	target.draw(shape);
}

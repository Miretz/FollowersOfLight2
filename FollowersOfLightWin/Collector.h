#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <SFML/Graphics.hpp>

class Collector
{
private:
	sf::CircleShape shape;

public:
	Collector();
	virtual ~Collector(void) = default;

	void setRadius(const float mRadius);

	void update(const sf::RenderWindow& window);
	void draw(sf::RenderTarget& target);

};


#endif COLLECTOR_H
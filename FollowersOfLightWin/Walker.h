#ifndef WALKER_H
#define WALKER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "RandomGenerator.h"

#ifndef _NOEXCEPT
#define _NOEXCEPT noexcept
#endif

typedef float FrameTime;

class Walker
{
protected:
private:

	float walkerWidth, walkerHeight, walkerVelocity, randomChance;

	sf::RectangleShape shape;
	sf::Vector2f velocity;
	sf::Vector3f color;

	void generateRandomColor();
	int generateRandomChance();
	float generateVelocityFloat();
	void normalize(sf::Vector2f& source);

	float lightAttenuation;
	bool lightUp = false;

public:

	static RandomGenerator gen;

	Walker(float mX, float mY);
	virtual ~Walker() = default;

	void update(FrameTime ft, sf::RenderWindow& window);
	void draw(sf::RenderTarget& target);
	
	sf::Vector2f getPosition() const _NOEXCEPT;
	sf::Vector3f getColor() const _NOEXCEPT;
	float getLightAttenuation() const _NOEXCEPT;

	float left()   const _NOEXCEPT;
	float right()  const _NOEXCEPT;
	float top()    const _NOEXCEPT;
	float bottom() const _NOEXCEPT;
};

#endif // WALKER_H

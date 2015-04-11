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

	const float walkerWidth = 6.f;
	const float walkerHeight = 6.f;
	const float walkerVelocity = 0.85f; 
	const float randomChance = 0.007f;

	float detectionRadius;
	float lightBPM;

	sf::RectangleShape shape;
	sf::Vector2f velocity;
	sf::Vector3f color;
	sf::Clock clock;

	void generateRandomColor();
	int generateRandomChance();
	float generateVelocityFloat();
	void normalize(sf::Vector2f& source);
		
public:

	static RandomGenerator gen;

	Walker(float mX, float mY, float mDetectionRadius, int mLightBPM);
	virtual ~Walker() = default;

	void update(FrameTime ft, sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader);
	
};

#endif // WALKER_H

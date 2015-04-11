#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "RandomGenerator.h"

typedef float FrameTime;

class Walker
{
private:

	const float walkerWidth = 6.f;
	const float walkerHeight = 6.f;
	const float walkerVelocity = 0.85f; 
	const float randomChance = 0.007f;

	float detectionRadius;
	bool selected;
	bool onTarget;
	int numCollistions = 0;
	
	sf::Vector2f targetPosition;

	sf::RectangleShape shape;
	sf::Vector2f velocity;
	sf::Vector3f color;
	
	sf::Vector3f generateRandomColor();
	int generateRandomChance();
	float generateVelocityFloat();
	void normalize(sf::Vector2f& source);
		
public:

	static RandomGenerator gen;

	Walker(sf::Vector2u winSize, float mDetectionRadius);
	virtual ~Walker(void) = default;

	void update(FrameTime ft, sf::Vector2u winSize, sf::Vector2f mousePosition);
	void draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader);

	void checkSelect(sf::Vector2f mousePosition);
	void setTargetPosition(sf::Vector2f targetPosition);
	void checkCollision(Walker& other);
	sf::Vector2f getPosition() const;
	
};

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <queue>
#include "RandomGenerator.h"

#define walkerWidth 40.f
#define walkerHeight 60.f
#define walkerVelocity 0.65f

class Walker
{
private:

	bool selected;

	int numCol = 0;

	std::queue<sf::Vector2f> targets;

	sf::Sprite sprite;

	sf::Vector2f velocity;
	sf::Vector3f color;
	
	sf::Vector3f generateRandomColor();
	void normalize(sf::Vector2f& source);
	bool checkSelect(sf::Vector2f mousePosition);
	void addTarget(const sf::Vector2f targetPosition);

public:

	static RandomGenerator gen;

	Walker(sf::Vector2u winSize, const sf::Texture& texture);
	virtual ~Walker(void) = default;

	void update(float ft, sf::Vector2u winSize, sf::Vector2f mousePosition);
	void draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader);
	void handle(sf::Event& event, sf::Vector2f mousepPosition);
	
	void checkCollision(const sf::FloatRect otherBounds, const sf::Vector2u winSize);
	sf::Vector2f getPosition() const;
	sf::FloatRect getBounds() const;
	
};

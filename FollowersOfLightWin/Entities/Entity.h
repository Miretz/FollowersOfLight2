#pragma once

#include <SFML/Graphics.hpp>
#include "../Utils/RandomGenerator.h"

class Entity
{
public:

	static RandomGenerator gen;

	bool selected{ false };
	
	virtual ~Entity() { }
	virtual void update(float ft) { }
	virtual void draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader) { }
	virtual void handle(const sf::Event& event, const sf::Vector2f& mousepPosition) { }
	virtual void checkCollision(const sf::FloatRect& otherBounds) { }
	
	virtual sf::Vector2f getPosition() const = 0;
	virtual sf::FloatRect getBounds() const = 0;

};

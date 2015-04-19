#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "RandomGenerator.h"

class Box : public Entity
{
private:
	sf::RectangleShape shape;

	sf::Vector2f walkerSize;
	sf::Vector2u winSize;

public:

	static RandomGenerator gen;

	Box(const sf::Vector2f& walkerPos, const sf::Vector2f& mWalkerSize, const sf::Texture& mTexture, const sf::Vector2u& mWinSize);
	virtual ~Box(void) = default;

	void update(float ft);
	void draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader);
	void handle(const sf::Event& event, const sf::Vector2f& mousepPosition);
	void checkCollision(const sf::FloatRect& otherBounds);

	sf::Vector2f getPosition() const;
	sf::FloatRect getBounds() const;

};
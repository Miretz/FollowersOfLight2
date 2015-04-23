#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Box : public Entity
{
private:
	sf::RectangleShape shape;

	sf::Vector2f walkerSize;
	sf::Vector2u winSize;

public:

	Box(const sf::Vector2f& walkerPos, const sf::Vector2f& mWalkerSize, const sf::Texture& mTexture, const sf::Vector2u& mWinSize);
	virtual ~Box(void) = default;

	void update(float ft) override;
	void draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader) override;
	void handle(const sf::Event& event, const sf::Vector2f& mousepPosition) override;
	void checkCollision(const sf::FloatRect& otherBounds) override;

	sf::Vector2f getPosition() const override;
	sf::FloatRect getBounds() const override;

};
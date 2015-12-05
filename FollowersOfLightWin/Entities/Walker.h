#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <memory>
#include <queue>

#include "Entity.h"

#define WALKER_VELOCITY_LIMIT 0.65f

class Walker : public Entity
{
private:

	sf::Sprite sprite;

	sf::Vector2f velocity;
	sf::Vector3f color;
	sf::Vector3f generateRandomColor();

	sf::Vector2f walkerSize;
	sf::Vector2u winSize;
	sf::Vector2f target;

	bool checkSelect(const sf::Vector2f& mousePosition);
	void addTarget(const sf::Vector2f targetPosition);

public:

	Walker(const sf::Vector2f& mWalkerSize, const sf::Texture& mTexture, const sf::Vector2u& mWinSize);
	virtual ~Walker(void) = default;

	void update(float ft) override;
	void draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader) override;
	void handle(const sf::Event& event, const sf::Vector2f& mousepPosition) override;
	void checkCollision(Entity* otherBounds) override;

	sf::Vector2f getPosition() const override;
	void setPosition(const sf::Vector2f position) override;
	sf::FloatRect getBounds() const override;
	
};

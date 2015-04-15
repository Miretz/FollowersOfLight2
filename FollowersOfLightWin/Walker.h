#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <queue>

#include "Entity.h"
#include "RandomGenerator.h"

#define WALKER_VELOCITY_LIMIT 0.65f

class Walker : public Entity
{
private:

	std::queue<sf::Vector2f> targets;

	sf::Sprite sprite;

	sf::Vector2f velocity;
	sf::Vector3f color;
	sf::Vector3f generateRandomColor();

	sf::Vector2f walkerSize;
	sf::Vector2u winSize;

	void normalize(sf::Vector2f& source);

	bool checkSelect(const sf::Vector2f& mousePosition);
	void addTarget(const sf::Vector2f targetPosition);

	int numCol = 0;

public:

	static RandomGenerator gen;

	Walker(const sf::Vector2f& mWalkerSize, const sf::Texture& mTexture, const sf::Vector2u& mWinSize);
	virtual ~Walker(void) = default;

	void update(float ft);
	void draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader);
	void handle(const sf::Event& event, const sf::Vector2f& mousepPosition);
	void checkCollision(const sf::FloatRect& otherBounds);

	sf::Vector2f getPosition() const;
	sf::FloatRect getBounds() const;
	
};

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

	sf::Sprite m_sprite;

	sf::Vector2f m_velocity;
	sf::Vector3f m_color;
	sf::Vector2f m_walkerSize;
	sf::Vector2u m_winSize;
	sf::Vector2f m_target;

	sf::Vector3f generateRandomColor();

	bool checkSelect(const sf::Vector2f& mousePosition);
	void addTarget(const sf::Vector2f targetPosition);

public:

	Walker(const sf::Vector2f& walkerSize, const sf::Texture& texture, const sf::Vector2u& winSize);
	virtual ~Walker(void) = default;

	void update(float ft) override;
	void draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader) override;
	void handle(const sf::Event& event, const sf::Vector2f& mousepPosition) override;
	void checkCollision(Entity* otherBounds) override;

	sf::Vector2f getPosition() const override;
	void setPosition(const sf::Vector2f position) override;
	sf::FloatRect getBounds() const override;
	const Type getType() const { return Type::WALKER; };
};

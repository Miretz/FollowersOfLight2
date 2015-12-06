#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.h"

class Box : public Entity
{
public:

	Box(const sf::Vector2f& walkerPos, const sf::Vector2f& mWalkerSize, const sf::Texture& mTexture, const sf::Vector2u& mWinSize);
	virtual ~Box(void) = default;

	void update(float ft) override;
	void draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader) override;
	void handle(const sf::Event& event, const sf::Vector2f& mousepPosition) override;
	void checkCollision(Entity* otherBounds) override;

	sf::Vector2f getPosition() const override;
	void setPosition(const sf::Vector2f position) override;
	sf::FloatRect getBounds() const override;
	const Type getType() const { return Type::BOX; };

private:
	sf::RectangleShape m_shape;
	sf::Vector2f m_walkerSize;
	sf::Vector2u m_winSize;

};
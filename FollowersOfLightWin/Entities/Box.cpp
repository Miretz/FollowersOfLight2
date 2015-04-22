#include "Box.h"

Box::Box(const sf::Vector2f& mWalkerPos, const sf::Vector2f& mWalkerSize, const sf::Texture& mTexture, const sf::Vector2u& mWinSize) : walkerSize(mWalkerSize), winSize(mWinSize)
{
	shape.setPosition(mWalkerPos.x, mWalkerPos.y);
	shape.setSize({ walkerSize.x, walkerSize.y });
	shape.setFillColor(sf::Color::Cyan);
	shape.setOrigin(walkerSize.x / 2.0f, walkerSize.y / 2.0f);

}

void Box::update(float ft)
{
}


void Box::draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader)
{
	target.draw(shape);
}

void Box::handle(const sf::Event& event, const sf::Vector2f& mousePosition)
{
}

void Box::checkCollision(const sf::FloatRect& otherBounds)
{
}

sf::FloatRect Box::getBounds() const
{
	return shape.getGlobalBounds();
}

sf::Vector2f Box::getPosition() const
{
	return shape.getPosition();
}

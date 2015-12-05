#include "Box.h"

#include "../Utils/VectorUtils.h"

Box::Box(const sf::Vector2f& mWalkerPos, const sf::Vector2f& mWalkerSize, const sf::Texture& mTexture, const sf::Vector2u& mWinSize) : walkerSize(mWalkerSize), winSize(mWinSize)
{
	shape.setPosition(mWalkerPos.x, mWalkerPos.y);
	shape.setSize({ walkerSize.x, walkerSize.y });
	shape.setFillColor(sf::Color::White);
	shape.setOrigin(walkerSize.x / 2.0f, walkerSize.y / 2.0f);
	
	shape.setTexture(&mTexture);

}

void Box::update(float ft)
{
}


void Box::draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader)
{
	shader->setParameter("frag_LightOrigin", shape.getPosition());
	shader->setParameter("frag_LightColor", sf::Vector3f(0.0f, 0.0f, 0.0f));
	shader->setParameter("frag_LightAttenuation", 50.f);

	sf::RenderStates states;
	states.shader = shader;
	states.blendMode = sf::BlendAdd;

	target.draw(spriteworld, states);

	target.draw(shape);
}

void Box::handle(const sf::Event& event, const sf::Vector2f& mousePosition)
{
}

void Box::checkCollision(Entity* other)
{
	sf::Vector2f distVec = getPosition() - other->getPosition();
	float distance = VectorUtils::length(distVec);

	float collisionDepth = walkerSize.x - distance;

	if (collisionDepth > 0.0f) {

		distVec = VectorUtils::normalize(distVec);
		sf::Vector2f collisionDepthVec = distVec * collisionDepth;

		shape.setPosition(shape.getPosition() + collisionDepthVec / 2.0f);
		other->setPosition(other->getPosition() - collisionDepthVec / 2.0f);

	}

}

void Box::setPosition(const sf::Vector2f position)
{
	shape.setPosition(position);
}

sf::FloatRect Box::getBounds() const
{
	return shape.getGlobalBounds();
}

sf::Vector2f Box::getPosition() const
{
	return shape.getPosition();
}

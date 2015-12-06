#include "PushableBox.h"

#include "../Utils/VectorUtils.h"

PushableBox::PushableBox(const sf::Vector2f& walkerPos, const sf::Vector2f& walkerSize, const sf::Texture& texture, const sf::Vector2u& winSize):
Box(walkerPos, walkerSize, texture, winSize)
{
	//Empty
}


void PushableBox::checkCollision(Entity* other) {

	if (other->getType() == Type::BOX)
	{
		return;
	}

	sf::Vector2f distVec = getPosition() - other->getPosition();
	float distance = VectorUtils::length(distVec);

	float collisionDepth = m_walkerSize.x - distance;

	if (collisionDepth > 0.0f)
	{

		distVec = VectorUtils::normalize(distVec);
		sf::Vector2f collisionDepthVec = distVec * collisionDepth;

		//shape.setPosition(shape.getPosition() + collisionDepthVec / 2.0f);
		//other->setPosition(other->getPosition() - collisionDepthVec / 2.0f);

		other->setPosition(other->getPosition() - collisionDepthVec);
	}

}

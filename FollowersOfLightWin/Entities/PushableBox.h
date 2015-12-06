#pragma once

#include "Box.h"

class PushableBox : public Box
{
public:

	PushableBox(const sf::Vector2f& walkerPos, const sf::Vector2f& walkerSize, const sf::Texture& texture, const sf::Vector2u& winSize);
	virtual ~PushableBox(void) = default;

	virtual void checkCollision(Entity* otherBounds) override;

	virtual const Type getType() const override { return Type::PUSHABLE_BOX; };
};
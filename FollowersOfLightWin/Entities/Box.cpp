#include "Box.h"

#include "../Utils/VectorUtils.h"

Box::Box(const sf::Vector2f& walkerPos, const sf::Vector2f& walkerSize, const sf::Texture& texture, const sf::Vector2u& winSize) : m_walkerSize(walkerSize), m_winSize(winSize)
{
	m_shape.setPosition(walkerPos.x, walkerPos.y);
	m_shape.setSize({ m_walkerSize.x, m_walkerSize.y });
	m_shape.setFillColor(sf::Color::White);
	m_shape.setOrigin(m_walkerSize.x / 2.0f, m_walkerSize.y / 2.0f);
	
	m_shape.setTexture(&texture);

}

void Box::update(float ft)
{
}


void Box::draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader)
{
	shader->setParameter("frag_LightOrigin", m_shape.getPosition());
	shader->setParameter("frag_LightColor", sf::Vector3f(0.0f, 0.0f, 0.0f));
	shader->setParameter("frag_LightAttenuation", 50.f);

	sf::RenderStates states;
	states.shader = shader;
	states.blendMode = sf::BlendAdd;

	target.draw(spriteworld, states);

	target.draw(m_shape);
}

void Box::handle(const sf::Event& event, const sf::Vector2f& mousePosition)
{
}

void Box::checkCollision(Entity* other)
{
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

void Box::setPosition(const sf::Vector2f position)
{
	m_shape.setPosition(position);
}

sf::FloatRect Box::getBounds() const
{
	return m_shape.getGlobalBounds();
}

sf::Vector2f Box::getPosition() const
{
	return m_shape.getPosition();
}

#include "Walker.h"

#include "../Utils/VectorUtils.h"

Walker::Walker(const sf::Vector2f& walkerSize, const sf::Texture& texture, const sf::Vector2u& winSize) : m_walkerSize(walkerSize), m_winSize(winSize)
{
	m_sprite.setTexture(texture);
	
	m_color = generateRandomColor();

	float mX = gen.randomFloat(0.f, static_cast<float>(m_winSize.x - m_walkerSize.x)) + m_walkerSize.x;
	float mY = gen.randomFloat(0.f, static_cast<float>(m_winSize.y - m_walkerSize.y)) + m_walkerSize.y;

	m_sprite.setPosition(mX, mY);

	m_target = m_sprite.getPosition();

	m_sprite.setOrigin(m_walkerSize.x / 2.0f, m_walkerSize.y / 2.0f);
}

void Walker::update(float ft)
{

	sf::Vector2f position = m_sprite.getPosition();
	sf::Vector2f distance = m_target - position;
	
	if ((abs(distance.x) < m_walkerSize.x) && (abs(distance.y) < m_walkerSize.y))
	{
		m_velocity.x = 0.f;
		m_velocity.y = 0.f;
		return;
	}

	distance = VectorUtils::normalize(distance);
	m_velocity = distance * WALKER_VELOCITY_LIMIT;
		
	//limit velocity
	m_velocity.x = std::min(WALKER_VELOCITY_LIMIT, m_velocity.x);
	m_velocity.y = std::min(WALKER_VELOCITY_LIMIT, m_velocity.y);
	m_velocity.x = std::max(-WALKER_VELOCITY_LIMIT, m_velocity.x);
	m_velocity.y = std::max(-WALKER_VELOCITY_LIMIT, m_velocity.y);

	float angle = std::atan2f(m_velocity.y, m_velocity.x);
	m_sprite.setRotation((angle * 57.2957795f) + 90);

	m_sprite.move(m_velocity * ft);

}


void Walker::draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader)
{

	if (selected)
	{
		//selection circle
		sf::CircleShape selector(m_walkerSize.x / 2.0f + 4.0f);
		selector.setPosition(getPosition());
		selector.setOrigin(selector.getRadius(), selector.getRadius());
		selector.setFillColor(sf::Color(0, 0, 0, 0));
		selector.setOutlineThickness(2);
		selector.setOutlineColor(sf::Color(50, 255, 50, 100));
		target.draw(selector);
	}

	shader->setParameter("frag_LightOrigin", m_sprite.getPosition());
	shader->setParameter("frag_LightColor", m_color);
	shader->setParameter("frag_LightAttenuation", 10.f);

	sf::RenderStates states;
	states.shader = shader;
	states.blendMode = sf::BlendAdd;

	target.draw(spriteworld, states);

	target.draw(m_sprite);

}

void Walker::checkCollision(Entity* other)
{
	
	if (other->getType() == Type::BOX)
	{
		return;
	}

	sf::Vector2f distVec = getPosition() - other->getPosition();
	float distance = VectorUtils::length(distVec);

	float collisionDepth = m_walkerSize.x - distance;

	if (collisionDepth > 0.0f) {

		distVec = VectorUtils::normalize(distVec);
		sf::Vector2f collisionDepthVec = distVec * collisionDepth;

		m_sprite.setPosition(m_sprite.getPosition() + collisionDepthVec / 2.0f);
		other->setPosition(other->getPosition() - collisionDepthVec / 2.0f);

	}

}

void Walker::setPosition(const sf::Vector2f position)
{
	m_sprite.setPosition(position);
}

void Walker::handle(const sf::Event& event, const sf::Vector2f& mousePosition)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			checkSelect(mousePosition);
		}
	}
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::A)
		{
			selected = true;
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		addTarget(mousePosition);
	}

}

bool Walker::checkSelect(const sf::Vector2f& mousePosition)
{

	sf::FloatRect boundingBox = m_sprite.getGlobalBounds();

	if (boundingBox.contains(mousePosition))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
			selected = !selected;
		}
		else {
			selected = true;
		}
	}
	else
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
			selected = false;
		}
	}

	return selected;

}

void Walker::addTarget(const sf::Vector2f mousePosition)
{
	if (selected)
	{
		m_velocity.x = 0.f;
		m_velocity.y = 0.f;
		m_target.x = mousePosition.x;
		m_target.y = mousePosition.y;
	}
}

sf::FloatRect Walker::getBounds() const
{
	return m_sprite.getGlobalBounds();
}

sf::Vector2f Walker::getPosition() const
{
	return m_sprite.getPosition();
}

sf::Vector3f Walker::generateRandomColor()
{
	float r = gen.randomFloat(0, 255);
	float g = gen.randomFloat(0, 255);
	float b = gen.randomFloat(0, 255);
	return sf::Vector3f(r, g, b);
}





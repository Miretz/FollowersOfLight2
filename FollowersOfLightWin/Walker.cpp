#include "Walker.h"

RandomGenerator Walker::gen = RandomGenerator();

Walker::Walker(float mX, float mY) : walkerWidth(6.f), walkerHeight(6.f), walkerVelocity(0.8f), randomChance(0.007f), lightAttenuation(50.f)
{
	generateRandomColor();

	shape.setPosition(mX, mY);
	shape.setSize({ walkerWidth, walkerHeight });
	shape.setFillColor(sf::Color(static_cast<sf::Uint8>(color.x), static_cast<sf::Uint8>(color.y), static_cast<sf::Uint8>(color.z)));
	shape.setOrigin(walkerWidth / 2.0f, walkerHeight / 2.0f);
}

void Walker::update(FrameTime ft, sf::RenderWindow& window)
{

	sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);
	sf::Vector2u winSize = window.getSize();
		
	//follow mouse if it is near the walker
	if (abs(mousePosition.x - shape.getPosition().x) < 150.f &&
		abs(mousePosition.y - shape.getPosition().y) < 150.f)
	{

		sf::Vector2f direction = mousePosition - shape.getPosition();
		normalize(direction);

		direction *= 0.01f;
		direction.x += generateVelocityFloat() * 0.05f;
		direction.y += generateVelocityFloat() * 0.05f;

		velocity += direction;
		//limit velocity
		velocity.x = std::min(walkerVelocity, velocity.x);
		velocity.y = std::min(walkerVelocity, velocity.y);
		velocity.x = std::max(-walkerVelocity, velocity.x);
		velocity.y = std::max(-walkerVelocity, velocity.y);

	}
	else
	{
		if (generateRandomChance())
		{
			velocity.x = generateVelocityFloat();
			velocity.y = generateVelocityFloat();
		}
	}

	//guard edges of screen
	if ((right() >= winSize.x && velocity.x > 0) || (left() <= 0 && velocity.x < 0))
	{
		velocity.x = 0;
	}
	if ((top() <= 0 && velocity.y < 0) || (bottom() >= winSize.y && velocity.y > 0))
	{
		velocity.y = 0;
	}

	shape.move(velocity * ft);

	//update lightAttenuation
	if (lightUp)
	{
		lightAttenuation += 0.1f;
	}
	else
	{
		lightAttenuation -= 0.1f;
	}
	if (lightAttenuation < 20.f || lightAttenuation > 50.f)
	{
		lightUp = !lightUp;
	}
}


void Walker::draw(sf::RenderTarget& target)
{
	target.draw(shape);
}

sf::Vector2f Walker::getPosition() const _NOEXCEPT
{
	return shape.getPosition();
}

sf::Vector3f Walker::getColor() const _NOEXCEPT
{
	return color;
}

float Walker::getLightAttenuation() const _NOEXCEPT
{
	return lightAttenuation;
}

float Walker::left()   const _NOEXCEPT
{
	return shape.getPosition().x - shape.getSize().x / 2.0f;
}

float Walker::right()  const _NOEXCEPT
{
	return shape.getPosition().x + shape.getSize().x / 2.0f;
}

float Walker::top()    const _NOEXCEPT
{
	return shape.getPosition().y - shape.getSize().y / 2.0f;
}

float Walker::bottom() const _NOEXCEPT
{
	return shape.getPosition().y + shape.getSize().y / 2.0f;
}

void Walker::generateRandomColor()
{
	float r = gen.randomFloat(0, 255);
	float g = gen.randomFloat(0, 255);
	float b = gen.randomFloat(0, 255);
	color = sf::Vector3f(r, g, b);
}

int Walker::generateRandomChance()
{
	return gen.randomChance(randomChance);
}

float Walker::generateVelocityFloat()
{
	return gen.randomFloat(-walkerVelocity, walkerVelocity);
}

void Walker::normalize(sf::Vector2f& source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
	{
		source.x = source.x / length;
		source.y = source.y / length;
	}
}



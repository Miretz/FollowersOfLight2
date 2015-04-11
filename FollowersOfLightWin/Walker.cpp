#include "Walker.h"

RandomGenerator Walker::gen = RandomGenerator();

Walker::Walker(float mX, float mY, float mdetectionRadius, int mLightBPM) : detectionRadius(mdetectionRadius)
{
	generateRandomColor();

	shape.setPosition(mX, mY);
	shape.setSize({ walkerWidth, walkerHeight });
	shape.setFillColor(sf::Color(static_cast<sf::Uint8>(color.x), static_cast<sf::Uint8>(color.y), static_cast<sf::Uint8>(color.z)));
	shape.setOrigin(walkerWidth / 2.0f, walkerHeight / 2.0f);

	lightBPM = (60.f / static_cast<float>(mLightBPM)) * 1000.f;

	clock.restart();

}

void Walker::update(FrameTime ft, sf::RenderWindow& window)
{

	sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);
	sf::Vector2u winSize = window.getSize();
		
	//follow mouse if it is near the walker
	if (abs(mousePosition.x - shape.getPosition().x) < detectionRadius &&
		abs(mousePosition.y - shape.getPosition().y) < detectionRadius)
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
	float top = shape.getPosition().y - shape.getSize().y / 2.0f;
	float bottom = shape.getPosition().y + shape.getSize().y / 2.0f;
	float left = shape.getPosition().x - shape.getSize().x / 2.0f;
	float right = shape.getPosition().x + shape.getSize().x / 2.0f;

	if ((right >= winSize.x && velocity.x > 0) || (left <= 0 && velocity.x < 0))
	{
		velocity.x = 0;
	}
	if ((top <= 0 && velocity.y < 0) || (bottom >= winSize.y && velocity.y > 0))
	{
		velocity.y = 0;
	}

	shape.move(velocity * ft);

}


void Walker::draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader)
{
	target.draw(shape);

	shader->setParameter("frag_LightOrigin", shape.getPosition());
	shader->setParameter("frag_LightColor", color);
	
	float elapsedMs = static_cast<float>(clock.getElapsedTime().asMilliseconds());

	if (elapsedMs > (lightBPM - 100.f))
	{
		shader->setParameter("frag_LightAttenuation", 50.f);

		if (elapsedMs > lightBPM){
			clock.restart();
		}

	}
	else
	{
		shader->setParameter("frag_LightAttenuation", 70.f);
	}

	sf::RenderStates states;
	states.shader = shader;
	states.blendMode = sf::BlendAdd;
	
	target.draw(spriteworld, states);

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



#include "Walker.h"

RandomGenerator Walker::gen = RandomGenerator();

Walker::Walker(sf::Vector2u winSize, float mdetectionRadius) : detectionRadius(mdetectionRadius), selected(false), onTarget(true)
{
	color = generateRandomColor();

	float mX = gen.randomFloat(0.f, static_cast<float>(winSize.x));
	float mY = gen.randomFloat(0.f, static_cast<float>(winSize.y));

	shape.setPosition(mX, mY);

	targetPosition = sf::Vector2f(mX, mY);	

	shape.setSize(sf::Vector2f(walkerWidth, walkerHeight));
	shape.setFillColor(sf::Color(static_cast<sf::Uint8>(color.x), static_cast<sf::Uint8>(color.y), static_cast<sf::Uint8>(color.z)));
	shape.setOrigin(walkerWidth / 2.0f, walkerHeight / 2.0f);

}

void Walker::update(FrameTime ft, sf::Vector2u winSize, sf::Vector2f mousePosition)
{
		
	if (!selected)
	{
		return;
	}

	sf::Vector2f direction = targetPosition - shape.getPosition();
	
	float minRadius = detectionRadius / 2.f;
	if ((abs(direction.x) < minRadius) && (abs(direction.y) < minRadius))
	{
		onTarget = true;
		velocity.x = 0.f;
		velocity.y = 0.f;
		return;
	}
	
	normalize(direction);

	direction *= 0.0015f;
	velocity += direction;
		
	//limit velocity
	velocity.x = std::min(walkerVelocity, velocity.x);
	velocity.y = std::min(walkerVelocity, velocity.y);
	velocity.x = std::max(-walkerVelocity, velocity.x);
	velocity.y = std::max(-walkerVelocity, velocity.y);

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

	if (selected)
	{
		sf::CircleShape selector(walkerWidth * 4.f);
		selector.setPosition(shape.getPosition());
		selector.setOrigin(walkerWidth * 4.f, walkerWidth * 4.f);
		selector.setFillColor(sf::Color(0, 0, 0, 0));
		selector.setOutlineThickness(2);
		selector.setOutlineColor(sf::Color(255, 255, 255, 100));
		selector.setPointCount(150);
		target.draw(selector);
	}

	shader->setParameter("frag_LightOrigin", shape.getPosition());
	shader->setParameter("frag_LightColor", color);
	shader->setParameter("frag_LightAttenuation", 50.f);
	
	sf::RenderStates states;
	states.shader = shader;
	states.blendMode = sf::BlendAdd;
	
	target.draw(spriteworld, states);

}

void Walker::checkSelect(sf::Vector2f mousePosition)
{
	if ((abs(mousePosition.x - shape.getPosition().x) < detectionRadius) &&
		(abs(mousePosition.y - shape.getPosition().y) < detectionRadius)){
		selected = true;
	}
	else
	{
		velocity.x = 0.f;
		velocity.y = 0.f;
		onTarget = true;
		targetPosition = shape.getPosition();
		selected = false;
	}
}

void Walker::setTargetPosition(sf::Vector2f mousePosition)
{
	if (selected)
	{
		targetPosition = mousePosition;
		onTarget = false;
	}
}

void Walker::checkCollision(Walker& other)
{
	
	if (!onTarget) return;

	float proximity = walkerWidth * 4.f;
	if ((abs(shape.getPosition().x - other.getPosition().x) < proximity) &&
		(abs(shape.getPosition().y - other.getPosition().y) < proximity))
	{
		
		float x = shape.getPosition().x < other.getPosition().x ? shape.getPosition().x - proximity : shape.getPosition().x + proximity;
		float y = shape.getPosition().y < other.getPosition().y ? shape.getPosition().y - proximity : shape.getPosition().y + proximity;
		shape.setPosition(x, y);

		numCollistions++;

		if (numCollistions < 200){
			numCollistions = 0;
			targetPosition.x = x;
			targetPosition.y = y;
		}

	}
}

sf::Vector2f Walker::getPosition() const
{
	return shape.getPosition();
}

sf::Vector3f Walker::generateRandomColor()
{
	float r = gen.randomFloat(0, 255);
	float g = gen.randomFloat(0, 255);
	float b = gen.randomFloat(0, 255);
	return sf::Vector3f(r, g, b);
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



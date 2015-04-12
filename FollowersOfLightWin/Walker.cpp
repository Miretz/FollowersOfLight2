#include "Walker.h"

RandomGenerator Walker::gen = RandomGenerator();

Walker::Walker(sf::Vector2u winSize, float mdetectionRadius, const sf::Texture& texture) : detectionRadius(mdetectionRadius), selected(false), onTarget(true)
{
	
	
	sprite.setTexture(texture);
	
	color = generateRandomColor();

	float mX = gen.randomFloat(0.f, static_cast<float>(winSize.x));
	float mY = gen.randomFloat(0.f, static_cast<float>(winSize.y));

	sprite.setPosition(mX, mY);

	targetPosition = sf::Vector2f(mX, mY);	

	//sprite.setSize(sf::Vector2f(walkerWidth, walkerHeight));
	//shape.setFillColor(sf::Color(static_cast<sf::Uint8>(color.x), static_cast<sf::Uint8>(color.y), static_cast<sf::Uint8>(color.z)));
	sprite.setOrigin(walkerWidth / 2.0f, walkerHeight / 2.0f);

}

void Walker::update(FrameTime ft, sf::Vector2u winSize, sf::Vector2f mousePosition)
{
		
	if (!selected)
	{
		return;
	}

	sf::Vector2f direction = targetPosition - sprite.getPosition();
	
	float minRadius = detectionRadius / 2.f;
	if ((abs(direction.x) < minRadius) && (abs(direction.y) < minRadius))
	{
		onTarget = true;
		velocity.x = 0.f;
		velocity.y = 0.f;
		return;
	}
	
	normalize(direction);

	direction *= 0.5f;
	velocity = direction;
		
	//limit velocity
	velocity.x = std::min(walkerVelocity, velocity.x);
	velocity.y = std::min(walkerVelocity, velocity.y);
	velocity.x = std::max(-walkerVelocity, velocity.x);
	velocity.y = std::max(-walkerVelocity, velocity.y);

	sprite.move(velocity * ft);

}


void Walker::draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader)
{
	target.draw(sprite);

	if (selected)
	{
		sf::CircleShape selector(detectionRadius);
		selector.setPosition(sprite.getPosition());
		selector.setOrigin(detectionRadius, detectionRadius);
		selector.setFillColor(sf::Color(0, 0, 0, 0));
		selector.setOutlineThickness(2);
		selector.setOutlineColor(sf::Color(255, 255, 255, 25));
		selector.setPointCount(150);
		target.draw(selector);
	}

	shader->setParameter("frag_LightOrigin", sprite.getPosition());
	shader->setParameter("frag_LightColor", color);
	shader->setParameter("frag_LightAttenuation", 30.f);

	sf::RenderStates states;
	states.shader = shader;
	states.blendMode = sf::BlendAdd;

	target.draw(spriteworld, states);

}

void Walker::checkSelect(sf::Vector2f mousePosition)
{
	if ((abs(mousePosition.x - sprite.getPosition().x) < detectionRadius) &&
		(abs(mousePosition.y - sprite.getPosition().y) < detectionRadius)){
		selected = true;
	}
	else
	{
		velocity.x = 0.f;
		velocity.y = 0.f;
		onTarget = true;
		targetPosition = sprite.getPosition();
		selected = false;
	}
}

void Walker::setTargetPosition(sf::Vector2f mousePosition)
{
	if (selected)
	{
		velocity.x = 0.f;
		velocity.y = 0.f;
		targetPosition = mousePosition;
		onTarget = false;
	}
}

void Walker::checkCollision(Walker& other)
{
	
	if (!onTarget) return;

	float proximity = walkerWidth;
	if ((abs(sprite.getPosition().x - other.getPosition().x) < proximity) &&
		(abs(sprite.getPosition().y - other.getPosition().y) < proximity))
	{
		
		float x = sprite.getPosition().x < other.getPosition().x ? sprite.getPosition().x - proximity : sprite.getPosition().x + proximity;
		float y = sprite.getPosition().y < other.getPosition().y ? sprite.getPosition().y - proximity : sprite.getPosition().y + proximity;
		sprite.setPosition(x, y);

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
	return sprite.getPosition();
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



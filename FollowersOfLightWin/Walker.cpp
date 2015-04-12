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

void Walker::update(float ft, sf::Vector2u winSize, sf::Vector2f mousePosition)
{
		
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

	sf::FloatRect boundingBox = sprite.getGlobalBounds();

	if (((boundingBox.left + walkerWidth) >= winSize.x && velocity.x > 0) || (boundingBox.left <= 0 && velocity.x < 0))
	{
		velocity.x = 0;
	}
	if ((boundingBox.top <= 0 && velocity.y < 0) || ((boundingBox.top + walkerHeight) >= winSize.y && velocity.y > 0))
	{
		velocity.y = 0;
	}
	
	sprite.move(velocity * ft);

}


void Walker::draw(sf::RenderTarget& target, sf::Sprite& spriteworld, sf::Shader* shader)
{
	target.draw(sprite);

	if (selected)
	{
		
		sf::FloatRect boundingBox = sprite.getGlobalBounds();
		
		sf::RectangleShape selector(sf::Vector2f(boundingBox.width, boundingBox.height));
		selector.setPosition(sprite.getPosition());
		selector.setOrigin(boundingBox.width / 2.f, boundingBox.height/2.f);
		selector.setFillColor(sf::Color(0, 0, 0, 0));
		selector.setOutlineThickness(2);
		selector.setOutlineColor(sf::Color(255, 255, 255, 25));
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

void Walker::handle(sf::Event& event, sf::Vector2f mousePosition)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			checkSelect(mousePosition);
		}
		if (event.mouseButton.button == sf::Mouse::Right)
		{
			setTargetPosition(mousePosition);
		}
	}
}


bool Walker::checkSelect(sf::Vector2f mousePosition)
{
	
	sf::FloatRect boundingBox = sprite.getGlobalBounds();

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

	sf::FloatRect boundingBox = sprite.getGlobalBounds();
	
	if (boundingBox.contains(other.getPosition()))
	{
		
		float x = sprite.getPosition().x < other.getPosition().x ? sprite.getPosition().x - boundingBox.width : sprite.getPosition().x + boundingBox.width;
		float y = sprite.getPosition().y < other.getPosition().y ? sprite.getPosition().y - boundingBox.height : sprite.getPosition().y + boundingBox.height;

		targetPosition.x = x;
		targetPosition.y = y;
		onTarget = false;


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

void Walker::normalize(sf::Vector2f& source)
{
	float length = sqrt((source.x * source.x) + (source.y * source.y));
	if (length != 0)
	{
		source.x = source.x / length;
		source.y = source.y / length;
	}
}



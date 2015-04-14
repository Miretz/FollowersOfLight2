#include "Walker.h"

RandomGenerator Walker::gen = RandomGenerator();

Walker::Walker(sf::Vector2u winSize, const sf::Texture& texture) : selected(false)
{
	
	
	sprite.setTexture(texture);
	
	color = generateRandomColor();

	float mX = gen.randomFloat(0.f, static_cast<float>(winSize.x));
	float mY = gen.randomFloat(0.f, static_cast<float>(winSize.y));

	sprite.setPosition(mX, mY);

	targets.push(sf::Vector2f(mX, mY));	

	//sprite.setSize(sf::Vector2f(walkerWidth, walkerHeight));
	//shape.setFillColor(sf::Color(static_cast<sf::Uint8>(color.x), static_cast<sf::Uint8>(color.y), static_cast<sf::Uint8>(color.z)));
	sprite.setOrigin(walkerWidth / 2.0f, walkerHeight / 2.0f);

}

void Walker::update(float ft, sf::Vector2u winSize, sf::Vector2f mousePosition)
{
	
	if (targets.empty()) return;

	sf::Vector2f targetPosition = targets.front();

	sf::Vector2f distance = targetPosition - sprite.getPosition();
	
	float minRadius = walkerWidth;
	if ((abs(distance.x) < minRadius) && (abs(distance.y) < minRadius))
	{
		targets.pop();
		velocity.x = 0.f;
		velocity.y = 0.f;
		return;
	}
	
	normalize(distance);

	sf::Vector2f speed = distance * walkerVelocity;
	velocity = speed;
		
	//limit velocity
	velocity.x = std::min(walkerVelocity, velocity.x);
	velocity.y = std::min(walkerVelocity, velocity.y);
	velocity.x = std::max(-walkerVelocity, velocity.x);
	velocity.y = std::max(-walkerVelocity, velocity.y);

	sprite.move(velocity * ft);

	sf::FloatRect boundingBox = sprite.getGlobalBounds();

	if ((boundingBox.left + walkerWidth) >= winSize.x && velocity.x > 0) 
	{
		sprite.setPosition(winSize.x - walkerWidth, getPosition().y);
		velocity.x = 0;
	}
	else if (boundingBox.left <= 0 && velocity.x < 0)
	{
		sprite.setPosition(0, getPosition().y);
		velocity.x = 0;
	}

	if (boundingBox.top <= 0 && velocity.y < 0) 
	{
		sprite.setPosition(getPosition().x, 0);
		velocity.y = 0;
	}
	else if ((boundingBox.top + walkerHeight) >= winSize.y && velocity.y > 0)
	{
		sprite.setPosition(getPosition().x, winSize.y - walkerHeight);
		velocity.y = 0;
	}
	
	

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
			addTarget(mousePosition);
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

void Walker::addTarget(const sf::Vector2f mousePosition)
{
	if (selected)
	{
		velocity.x = 0.f;
		velocity.y = 0.f;
		targets.push(mousePosition);
	}
}

void Walker::checkCollision(const sf::FloatRect otherBounds, const sf::Vector2u winSize)
{
	
	sf::FloatRect boundingBox = sprite.getGlobalBounds();
	
	if (boundingBox.intersects(otherBounds))
	{
		float newX = sprite.getPosition().x;
		float newY = sprite.getPosition().y;

		if (boundingBox.left > otherBounds.left){
			newX += walkerWidth;
		}
		if (boundingBox.top > otherBounds.top){
			newY += walkerHeight;
		}
		if (boundingBox.left < otherBounds.left){
			newX -= walkerWidth;
		}
		if (boundingBox.top < otherBounds.top){
			newY -= walkerHeight;
		}

		if (numCol > 10){
			while (!targets.empty())
			{
				targets.pop();
			}
			numCol = 0;
			targets.push(sf::Vector2f(newX, newY));
		} 
		else {
			numCol++;
			
			if (numCol == 5)
			{
				std::queue<sf::Vector2f> newTargets;
				newTargets.push(sf::Vector2f(newX, newY));
				while (!targets.empty())
				{
					newTargets.push(targets.front());
					targets.pop();
				}
				targets.swap(newTargets);
			}
		}
	}
}

sf::FloatRect Walker::getBounds() const
{
	return sprite.getGlobalBounds();
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



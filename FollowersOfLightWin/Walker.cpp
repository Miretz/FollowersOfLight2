#include "Walker.h"

RandomGenerator Walker::gen = RandomGenerator();

Walker::Walker(const sf::Vector2f& mWalkerSize, const sf::Texture& mTexture, const sf::Vector2u& mWinSize) : walkerSize(mWalkerSize), winSize(mWinSize)
{
	sprite.setTexture(mTexture);
	
	color = generateRandomColor();

	float mX = gen.randomFloat(0.f, static_cast<float>(winSize.x));
	float mY = gen.randomFloat(0.f, static_cast<float>(winSize.y));

	sprite.setPosition(mX, mY);

	sprite.setOrigin(walkerSize.x / 2.0f, walkerSize.y / 2.0f);
}

void Walker::update(float ft)
{
	if (targets.empty()) return;

	sf::Vector2f targetPosition = targets.front();

	sf::Vector2f distance = targetPosition - sprite.getPosition();
	
	float minRadius = 5.f;
	if ((abs(distance.x) < minRadius) && (abs(distance.y) < minRadius))
	{
		targets.pop();
		velocity.x = 0.f;
		velocity.y = 0.f;
		return;
	}
	
	normalize(distance);

	velocity = distance * WALKER_VELOCITY_LIMIT;
		
	//limit velocity
	velocity.x = std::min(WALKER_VELOCITY_LIMIT, velocity.x);
	velocity.y = std::min(WALKER_VELOCITY_LIMIT, velocity.y);
	velocity.x = std::max(-WALKER_VELOCITY_LIMIT, velocity.x);
	velocity.y = std::max(-WALKER_VELOCITY_LIMIT, velocity.y);

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

void Walker::handle(const sf::Event& event, const sf::Vector2f& mousePosition)
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
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::A)
		{
			selected = true;
		}
	}
}

void Walker::checkCollision(const sf::FloatRect& otherBounds)
{
	sf::FloatRect boundingBox = sprite.getGlobalBounds();

	if (boundingBox.intersects(otherBounds))
	{
		numCol++;

		if (numCol > 10000)
		{
			while (!targets.empty()) targets.pop();
			numCol = 0;
		}

		if (numCol % 10 == 0)
		{
			float newX = sprite.getPosition().x;
			float newY = sprite.getPosition().y;

			if (numCol % 20 == 0)
			{
				if (boundingBox.left >= otherBounds.left){
					newX = otherBounds.left + (walkerSize.x);
				}
				else if (boundingBox.left < otherBounds.left){
					newX = otherBounds.left - (walkerSize.x);
				}
			}
			else 
			{
				if (boundingBox.top >= otherBounds.top){
					newY = otherBounds.top + (walkerSize.y);
				}
				else if (boundingBox.top < otherBounds.top){
					newY = otherBounds.top - (walkerSize.y);
				}
			}
			
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


bool Walker::checkSelect(const sf::Vector2f& mousePosition)
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
		//while (!targets.empty()) targets.pop();
		
		velocity.x = 0.f;
		velocity.y = 0.f;
		targets.push(mousePosition);
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



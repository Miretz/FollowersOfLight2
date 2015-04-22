#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

class TextureHandler
{
public:
	static sf::Texture* getTexture(const std::string& path);

private:
	class TextureObject
	{
	public:
		TextureObject();
		~TextureObject();
		sf::Texture* getTexture() const;
		bool loadTextureFromFile(const std::string& path);

	private:
		sf::Texture* mTexture;
	};

	typedef std::map<std::string, TextureObject> TexturesMap;
	static TexturesMap myTextures;
};
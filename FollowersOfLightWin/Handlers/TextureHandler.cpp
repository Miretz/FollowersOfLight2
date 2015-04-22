#include "TextureHandler.h"

TextureHandler::TexturesMap TextureHandler::myTextures;

sf::Texture* TextureHandler::getTexture(const std::string& path)
{
	TexturesMap::iterator it = myTextures.find(path);

	if (it != myTextures.end())
		return it->second.getTexture();

	if (!myTextures[path].loadTextureFromFile(path))
		std::cout << "An error occured while loading the following texture:" << path << std::endl;

	return myTextures[path].getTexture();
}

TextureHandler::TextureObject::TextureObject()
{
	mTexture = NULL;
}

TextureHandler::TextureObject::~TextureObject()
{
	if (mTexture)
		delete mTexture;
}

sf::Texture* TextureHandler::TextureObject::getTexture() const
{
	return mTexture;
};

bool TextureHandler::TextureObject::loadTextureFromFile(const std::string& path)
{
	if (!mTexture)
		mTexture = new sf::Texture;

	return mTexture->loadFromFile(path);
}

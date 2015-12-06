#include "TextureHandler.h"

TextureHandler::TexturesMap TextureHandler::m_textures;

sf::Texture* TextureHandler::getTexture(const std::string& path)
{
	TexturesMap::iterator it = m_textures.find(path);

	if (it != m_textures.end())
		return it->second.getTexture();

	if (!m_textures[path].loadTextureFromFile(path))
		std::cout << "An error occured while loading the following texture:" << path << std::endl;

	return m_textures[path].getTexture();
}

TextureHandler::TextureObject::TextureObject()
{
	m_texture = NULL;
}

TextureHandler::TextureObject::~TextureObject()
{
	if (m_texture)
		delete m_texture;
}

sf::Texture* TextureHandler::TextureObject::getTexture() const
{
	return m_texture;
};

bool TextureHandler::TextureObject::loadTextureFromFile(const std::string& path)
{
	if (!m_texture)
		m_texture = new sf::Texture;

	return m_texture->loadFromFile(path);
}

#include "ShaderHandler.h"

ShaderHandler::ShadersMap ShaderHandler::myShaders;

sf::Shader* ShaderHandler::getShader(const std::string& path)
{
	ShadersMap::iterator it = myShaders.find(path);

	if (it != myShaders.end())
		return it->second.getShader();

	if (!myShaders[path].loadShaderFromFile(path))
		std::cout << "An error occured while loading the following shader:" << path << std::endl;

	return myShaders[path].getShader();
}

ShaderHandler::ShaderObject::ShaderObject()
{
	myShader = NULL;
}

ShaderHandler::ShaderObject::~ShaderObject()
{
	if (myShader)
		delete myShader;
}

sf::Shader* ShaderHandler::ShaderObject::getShader() const
{
	return myShader;
};

bool ShaderHandler::ShaderObject::loadShaderFromFile(const std::string& path)
{
	if (!myShader)
		myShader = new sf::Shader;

	return myShader->loadFromFile(path, sf::Shader::Fragment);
}

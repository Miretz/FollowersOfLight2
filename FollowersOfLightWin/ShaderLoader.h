#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>


class ShaderLoader
{
public:
	static sf::Shader* getShader(const std::string& path);

private:
	class ShaderObject
	{
	public:
		ShaderObject();
		~ShaderObject();
		sf::Shader* getShader() const;
		bool loadShaderFromFile(const std::string& path);

	private:
		sf::Shader* myShader;
	};

	typedef std::map<std::string, ShaderObject> ShadersMap;
	static ShadersMap myShaders;
};
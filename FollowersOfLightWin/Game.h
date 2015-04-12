#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Walker.h"
#include "ShaderLoader.h"

#define ftStep 1.f
#define ftSlice 1.f

#define windowWidth 1024
#define windowHeight 768
#define walkerCount 15

#define detectionRadius 80.f

class Game
{
private:
	
	float lastFt = 0.f;
	float currentSlice = 0.f;

	sf::RenderWindow window;
	sf::RenderTexture myRenderTexture;
	sf::Sprite spriteWorld;
	sf::Shader* shader;

	bool running;

	std::vector<Walker> walkers;

	void initializeWalkers();
	void checkInput();
	void checkCollisions();
	void update();
	void draw();

	sf::Texture texture;

public:
	Game();
	virtual ~Game() = default;
	void run();
};

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Walker.h"
#include "ShaderLoader.h"
#include "Collector.h"

typedef float FrameTime;

class Game
{
private:
	
	const float ftStep = 1.f;
	const float ftSlice = 1.f;
	FrameTime lastFt = 0.f;
	FrameTime currentSlice = 0.f;

	unsigned int windowWidth;
	unsigned int windowHeight;
	int walkerCount;

	sf::RenderWindow window;
	sf::RenderTexture myRenderTexture;
	sf::Sprite spriteWorld;

	sf::Shader* shader;

	bool running;

	std::vector<Walker> walkers;
	Collector collector;

	float detectionRadius;

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

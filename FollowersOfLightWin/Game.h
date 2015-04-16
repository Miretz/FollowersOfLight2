#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "Entity.h"
#include "Walker.h"
#include "ShaderLoader.h"
#include "Tilemap.h"

#define FTSTEP 1.f
#define FTSLICE 1.f

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WALKER_COUNT 15

#define DETECTION_RADIUS 80.f

#define WALKER_WIDTH 32
#define WALKER_HEIGHT 32


class Game
{
private:
	
	float lastFt = 0.f;
	float currentSlice = 0.f;
	bool running;

	sf::RenderWindow window;
	sf::RenderTexture myRenderTexture;
	sf::Sprite spriteWorld;
	sf::Shader* shader;

	std::vector<std::unique_ptr<Entity>> entities;

	void initializeWalkers();
	void checkInput();
	void update();
	void draw();
	void drawGrid();
	void checkCollisions();

	sf::Texture texture;

	Tilemap map;

public:
	Game();
	virtual ~Game() = default;
	void run();
};

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "Entities/Entity.h"
#include "Entities/Walker.h"
#include "Entities/Box.h"

#include "Handlers/ShaderHandler.h"
#include "Handlers/WindowHandler.h"

#include "Utils/Tilemap.h"

#define FTSTEP 1.f
#define FTSLICE 1.f

#define WALKER_COUNT 6
#define BOX_COUNT 16

#define DETECTION_RADIUS 80.f

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

//note change this afterwards
#define SHADER_PATH "../FollowersOfLightWin/Resources/shader.frag"
#define TILEMAP_PATH "../FollowersOfLightWin/Resources/Tilemap.png"
#define WALKER_TEXTURE_PATH "../FollowersOfLightWin/Resources/Walker.png"



class Game
{
	float lastFt = 0.f;
	float currentSlice = 0.f;
	bool running;

	sf::RenderWindow* window;
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

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "Entities/Entity.h"
#include "Entities/Walker.h"
#include "Entities/Box.h"
#include "Entities/PushableBox.h"

#include "Handlers/ShaderHandler.h"
#include "Handlers/TextureHandler.h"
#include "Handlers/WindowHandler.h"

#include "Utils/Tilemap.h"

#define FTSTEP 1.f
#define FTSLICE 1.f

#define WALKER_COUNT 6

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define SHADER_PATH "Resources/shader.frag"
#define TILEMAP_PATH "Resources/Tilemap.png"
#define WALKER_TEXTURE_PATH "Resources/Walker.png"
#define BOX_TEXTURE_PATH "Resources/Box.png"
#define PUSHABLE_BOX_TEXTURE_PATH "Resources/PushableBox.png"

#define LEVEL_WIDTH 16
#define LEVEL_HEIGHT 12

class Game
{
public:
	Game();
	virtual ~Game() = default;
	void run();

private:
	const static int m_level[];

	float m_lastFt = 0.f;
	float m_currentSlice = 0.f;
	bool m_running;

	sf::RenderWindow* m_window;
	sf::RenderTexture m_myRenderTexture;
	sf::Sprite m_spriteWorld;
	sf::Shader* m_shader;

	std::vector<std::unique_ptr<Entity>> m_entities;
	
	sf::Texture* m_walkerTexture;
	sf::Texture* m_boxTexture;
	sf::Texture* m_pushableBoxTexture;

	Tilemap m_map;

	void initializeWalkers();
	void checkInput();
	void update();
	void draw();
	void drawGrid();
	void checkCollisions();

};

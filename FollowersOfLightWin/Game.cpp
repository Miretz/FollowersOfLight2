#include "Game.h"

const int Game::m_level[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 4, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 0, 0,
	0, 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0, 0,
	0, 0, 1, 4, 3, 4, 1, 5, 1, 5, 1, 3, 2, 4, 0, 0,
	0, 0, 1, 2, 3, 1, 1, 1, 1, 1, 1, 3, 2, 1, 0, 0,
	0, 0, 1, 2, 3, 3, 3, 4, 3, 3, 3, 3, 2, 4, 0, 0,
	0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 4, 1, 4, 1, 4, 1, 4, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

Game::Game() : m_running(false)
{
	
	//create window singleton instance
	m_window = WindowHandler::Instance()->getWindow();

	//load texture walker
	m_walkerTexture = TextureHandler::getTexture(WALKER_TEXTURE_PATH);
	m_walkerTexture->setSmooth(true);
	
	//load texture box
	m_boxTexture = TextureHandler::getTexture(BOX_TEXTURE_PATH);
	m_boxTexture->setSmooth(true);

	//load pushable box texture
	m_pushableBoxTexture = TextureHandler::getTexture(PUSHABLE_BOX_TEXTURE_PATH);
	m_pushableBoxTexture->setSmooth(true);

	//load shader
	m_shader = ShaderHandler::getShader(SHADER_PATH);
	m_shader->setParameter("frag_ScreenResolution", sf::Vector2f(m_window->getSize()));
	
	if (!m_map.load(TILEMAP_PATH, sf::Vector2u(TILE_WIDTH, TILE_HEIGHT), m_level, LEVEL_WIDTH, LEVEL_HEIGHT))
		std::cout << "Error loading tilemap!" << std::endl;

}

void Game::run()
{
	
	m_window->setVerticalSyncEnabled(true);
	initializeWalkers();
	
	m_myRenderTexture.create(m_window->getSize().x, m_window->getSize().y);

	m_spriteWorld.setTexture(m_myRenderTexture.getTexture());
	m_spriteWorld.setOrigin(m_spriteWorld.getTextureRect().width / 2.f, m_spriteWorld.getTextureRect().height / 2.f);
	m_spriteWorld.setPosition(m_window->getSize().x / 2.f, m_window->getSize().y / 2.f);
	
	sf::Clock clock;

	m_running = true;

	while (m_running)
	{

		m_window->clear();
		m_myRenderTexture.clear(sf::Color(0, 0, 65));

		checkInput();
		update();
		draw();

		sf::Int32 elapsedTime = clock.restart().asMilliseconds();
		float ft = static_cast<float>(elapsedTime);

		m_lastFt = ft;

		float ftSeconds = ft / 1000.f;
		if (ftSeconds > 0.f)
		{
			float fps = 1.f / ftSeconds;
			m_window->setTitle("FT: " + std::to_string(ft) + "\tFPS: " + std::to_string(fps));
		}
	}
}

void Game::initializeWalkers()
{
	for (int a = 0; a < WALKER_COUNT; ++a)
	{
		m_entities.emplace_back(std::make_unique<Walker>(sf::Vector2f(TILE_WIDTH, TILE_HEIGHT), *m_walkerTexture, m_window->getSize()));
	}
	
	int levelSize = (sizeof(m_level) / sizeof(*m_level));
	for (int a = 0; a < levelSize; ++a)
	{
		if (m_level[a] == 4){

			int x = a % LEVEL_WIDTH;
			int y = a / LEVEL_WIDTH;

			sf::Vector2f posVec(((x + 1) * TILE_WIDTH) - (TILE_WIDTH / 2.0f), ((y + 1) * TILE_HEIGHT) - (TILE_WIDTH / 2.0f));
			m_entities.emplace_back(std::make_unique<Box>(posVec, sf::Vector2f(TILE_WIDTH, TILE_HEIGHT), *m_boxTexture, m_window->getSize()));
		}

		if (m_level[a] == 5){

			int x = a % LEVEL_WIDTH;
			int y = a / LEVEL_WIDTH;

			sf::Vector2f posVec(((x + 1) * TILE_WIDTH) - (TILE_WIDTH / 2.0f), ((y + 1) * TILE_HEIGHT) - (TILE_WIDTH / 2.0f));
			m_entities.emplace_back(std::make_unique<PushableBox>(posVec, sf::Vector2f(TILE_WIDTH, TILE_HEIGHT), *m_pushableBoxTexture, m_window->getSize()));
		}
	}
}

void Game::checkInput()
{
	sf::Event event;
	while (m_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}
		else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
		{
			m_running = false;
		}
		else {

			sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(*m_window);

			for (auto& entity : m_entities)
			{
				entity->handle(event, mousePosition);
			}
		}
	}
		
}

void Game::update()
{
	m_currentSlice += m_lastFt;

	sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(*m_window);

	//update multiple times based on fps
	for (; m_currentSlice >= FTSLICE; m_currentSlice -= FTSLICE)
	{
		for (auto& entity : m_entities)
		{
			entity->update(FTSTEP);
		}

	}

	checkCollisions();
	
}

void Game::draw()
{
	
	m_myRenderTexture.draw(m_map);

	//draw grid
	//drawGrid();

	for (auto& entity : m_entities)
	{

		entity->draw(m_myRenderTexture, m_spriteWorld, m_shader);
	}
	
	m_myRenderTexture.display();
	m_window->draw(m_spriteWorld);
	m_window->display();
}

void Game::drawGrid()
{
	for (unsigned int i = 0; i < m_window->getSize().x; ++i)
	{
		if ((i % TILE_WIDTH) == 0)
		{
			float ifL = static_cast<float>(i);
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(ifL, 0.f), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f)),
				sf::Vertex(sf::Vector2f(ifL, m_window->getSize().y), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f))
			};
			m_myRenderTexture.draw(line, 2, sf::Lines);
		}
	}
	for (unsigned int i = 0; i < m_window->getSize().y; ++i)
	{
		if ((i % TILE_HEIGHT) == 0)
		{
			float ifL = static_cast<float>(i);
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(0.f, ifL), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f)),
				sf::Vertex(sf::Vector2f(m_window->getSize().x, ifL), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f))
			};
			m_myRenderTexture.draw(line, 2, sf::Lines);
		}
	}
}


void Game::checkCollisions(){
	
	static const float edge = TILE_WIDTH / 2;

	//prevent leaving out of screen
	for (auto& entity : m_entities){
		if (entity->getPosition().x < 0 + edge){ entity->setPosition(sf::Vector2f(edge, entity->getPosition().y)); }
		if (entity->getPosition().y < 0 + edge){ entity->setPosition(sf::Vector2f(entity->getPosition().x, edge)); }
		if (entity->getPosition().x > m_window->getSize().x - edge){ entity->setPosition(sf::Vector2f(m_window->getSize().x - edge, entity->getPosition().y)); }
		if (entity->getPosition().y > m_window->getSize().y - edge){ entity->setPosition(sf::Vector2f(entity->getPosition().x, m_window->getSize().y - edge)); }
	}
	
	for (auto& entityA : m_entities){
		for (auto& entityB : m_entities){
			if (entityA != entityB)
			{
				entityA->checkCollision(entityB.get());
			}
		}
	}
}
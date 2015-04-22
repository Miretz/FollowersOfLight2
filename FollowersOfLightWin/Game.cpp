#include "Game.h"

WindowHandler* WindowHandler::s_pInstance = nullptr;

Game::Game() : running(false)
{
	
	//create window singleton instance
	window = WindowHandler::Instance()->getWindow();

	//load texture
	if (!texture.loadFromFile(WALKER_TEXTURE_PATH))
	{
		std::cout << "Texture error!" << std::endl;
	}
	texture.setSmooth(true);
	
	//load shader
	shader = ShaderHandler::getShader(SHADER_PATH);
	shader->setParameter("frag_ScreenResolution", sf::Vector2f(window->getSize()));

	//load tilemap
	const int level[] =
	{
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
		0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
	};
	
	if (!map.load(TILEMAP_PATH, sf::Vector2u(64, 64), level, 16, 12))
		std::cout << "Error loading tilemap!" << std::endl;

}

void Game::run()
{
	
	window->setVerticalSyncEnabled(true);
	initializeWalkers();
	
	myRenderTexture.create(window->getSize().x, window->getSize().y);

	spriteWorld.setTexture(myRenderTexture.getTexture());
	spriteWorld.setOrigin(spriteWorld.getTextureRect().width / 2.f, spriteWorld.getTextureRect().height / 2.f);
	spriteWorld.setPosition(window->getSize().x / 2.f, window->getSize().y / 2.f);
	
	sf::Clock clock;

	running = true;

	while (running)
	{

		window->clear();
		myRenderTexture.clear(sf::Color(0, 0, 65));

		checkInput();
		update();
		draw();

		sf::Int32 elapsedTime = clock.restart().asMilliseconds();
		float ft = static_cast<float>(elapsedTime);

		lastFt = ft;

		float ftSeconds = ft / 1000.f;
		if (ftSeconds > 0.f)
		{
			float fps = 1.f / ftSeconds;
			window->setTitle("FT: " + std::to_string(ft) + "\tFPS: " + std::to_string(fps));
		}
	}
}

void Game::initializeWalkers()
{
	for (int a = 0; a < WALKER_COUNT; ++a)
	{
		entities.emplace_back(std::make_unique<Walker>(sf::Vector2f(TILE_WIDTH, TILE_HEIGHT), texture, window->getSize()));
	}
	
	for (int a = 0; a < BOX_COUNT; ++a)
	{
		if (a != 4)
		{
			entities.emplace_back(std::make_unique<Box>(sf::Vector2f(((a + 1) * TILE_WIDTH) - (TILE_WIDTH / 2.0f), (10 * TILE_HEIGHT) - (TILE_WIDTH / 2.0f)), sf::Vector2f(TILE_WIDTH, TILE_HEIGHT), texture, window->getSize()));
		}
	}
}

void Game::checkInput()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			running = false;
		}
		else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
		{
			running = false;
		}
		else {

			sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(*window);

			for (auto& entity : entities)
			{
				entity->handle(event, mousePosition);
			}
		}
	}
		
}

void Game::update()
{
	currentSlice += lastFt;

	sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(*window);

	//update multiple times based on fps
	for (; currentSlice >= FTSLICE; currentSlice -= FTSLICE)
	{
		for (auto& entity : entities)
		{
			entity->update(FTSTEP);
		}

	}

	checkCollisions();
	
}

void Game::draw()
{
	
	myRenderTexture.draw(map);

	//draw grid
	drawGrid();

	for (auto& entity : entities)
	{

		entity->draw(myRenderTexture, spriteWorld, shader);
	}
	
	myRenderTexture.display();
	window->draw(spriteWorld);
	window->display();
}

void Game::drawGrid()
{
	for (unsigned int i = 0; i < window->getSize().x; ++i)
	{
		if ((i % TILE_WIDTH) == 0)
		{
			float ifL = static_cast<float>(i);
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(ifL, 0.f), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f)),
				sf::Vertex(sf::Vector2f(ifL, window->getSize().y), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f))
			};
			myRenderTexture.draw(line, 2, sf::Lines);
		}
	}
	for (unsigned int i = 0; i < window->getSize().y; ++i)
	{
		if ((i % TILE_HEIGHT) == 0)
		{
			float ifL = static_cast<float>(i);
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(0.f, ifL), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f)),
				sf::Vertex(sf::Vector2f(window->getSize().x, ifL), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f))
			};
			myRenderTexture.draw(line, 2, sf::Lines);
		}
	}
}


void Game::checkCollisions(){
	for (auto& entityA : entities){
		for (auto& entityB : entities){
			if (entityA != entityB)
			{
				entityA->checkCollision(entityB->getBounds());
			}
		}
	}
}
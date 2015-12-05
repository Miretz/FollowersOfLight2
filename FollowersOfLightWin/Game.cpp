#include "Game.h"

const int Game::level[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 0, 0,
	0, 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0, 0,
	0, 0, 1, 2, 3, 1, 4, 1, 1, 4, 1, 3, 2, 1, 0, 0,
	0, 0, 1, 2, 3, 1, 1, 1, 1, 1, 1, 3, 2, 1, 0, 0,
	0, 0, 1, 2, 3, 3, 4, 3, 3, 3, 3, 3, 2, 1, 0, 0,
	0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0,
	0, 0, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0, 0,
	0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

Game::Game() : running(false)
{
	
	//create window singleton instance
	window = WindowHandler::Instance()->getWindow();

	//load texture walker
	walkerTexture = TextureHandler::getTexture(WALKER_TEXTURE_PATH);
	walkerTexture->setSmooth(true);
	
	//load texture box
	boxTexture = TextureHandler::getTexture(BOX_TEXTURE_PATH);
	boxTexture->setSmooth(true);

	//load shader
	shader = ShaderHandler::getShader(SHADER_PATH);
	shader->setParameter("frag_ScreenResolution", sf::Vector2f(window->getSize()));
	
	if (!map.load(TILEMAP_PATH, sf::Vector2u(TILE_WIDTH, TILE_HEIGHT), level, LEVEL_WIDTH, LEVEL_HEIGHT))
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
		entities.emplace_back(std::make_unique<Walker>(sf::Vector2f(TILE_WIDTH, TILE_HEIGHT), *walkerTexture, window->getSize()));
	}
	
	int levelSize = (sizeof(level) / sizeof(*level));
	for (int a = 0; a < levelSize; ++a)
	{
		if (level[a] == 4){

			int x = a % LEVEL_WIDTH;
			int y = a / LEVEL_WIDTH;

			sf::Vector2f posVec(((x + 1) * TILE_WIDTH) - (TILE_WIDTH / 2.0f), ((y + 1) * TILE_HEIGHT) - (TILE_WIDTH / 2.0f));
			entities.emplace_back(std::make_unique<Box>(posVec, sf::Vector2f(TILE_WIDTH, TILE_HEIGHT), *boxTexture, window->getSize()));
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
	//drawGrid();

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
	
	static const float edge = TILE_WIDTH / 2;

	//prevent leaving out of screen
	for (auto& entity : entities){
		if (entity->getPosition().x < 0 + edge){ entity->setPosition(sf::Vector2f(edge, entity->getPosition().y)); }
		if (entity->getPosition().y < 0 + edge){ entity->setPosition(sf::Vector2f(entity->getPosition().x, edge)); }
		if (entity->getPosition().x > window->getSize().x - edge){ entity->setPosition(sf::Vector2f(window->getSize().x - edge, entity->getPosition().y)); }
		if (entity->getPosition().y > window->getSize().y - edge){ entity->setPosition(sf::Vector2f(entity->getPosition().x, window->getSize().y - edge)); }
	}
	
	for (auto& entityA : entities){
		for (auto& entityB : entities){
			if (entityA != entityB)
			{
				entityA->checkCollision(entityB.get());
			}
		}
	}
}
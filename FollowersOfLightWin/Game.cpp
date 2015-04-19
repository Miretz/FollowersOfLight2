#include "Game.h"

Game::Game() : running(false)
{
	
	//load texture
	if (!texture.loadFromFile("../Walker.png"))
	{
		std::cout << "Texture error!" << std::endl;
	}
	texture.setSmooth(true);
	
	//load shader
	shader = ShaderLoader::getShader("shader.frag");
	shader->setParameter("frag_ScreenResolution", sf::Vector2f(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)));

	//load tilemap
	const int level[] =
	{
		1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		1, 1, 1, 1, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3, 3,
		1, 3, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1,
		3, 3, 0, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1,
		3, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		3, 0, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		3, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		3, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
		3, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1,
		3, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1,
		3, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};
	
	if (!map.load("../Tilemap.png", sf::Vector2u(64, 64), level, 16, 12))
		std::cout << "Error loading tilemap!" << std::endl;

}

void Game::run()
{
	
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Followers!");
	window.setVerticalSyncEnabled(true);
	initializeWalkers();
	
	myRenderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);

	spriteWorld.setTexture(myRenderTexture.getTexture());
	spriteWorld.setOrigin(spriteWorld.getTextureRect().width / 2.f, spriteWorld.getTextureRect().height / 2.f);
	spriteWorld.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
	
	sf::Clock clock;

	running = true;

	while (running)
	{

		window.clear();
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
			window.setTitle("FT: " + std::to_string(ft) + "\tFPS: " + std::to_string(fps));
		}
	}
}

void Game::initializeWalkers()
{
	for (int a = 0; a < WALKER_COUNT; ++a)
	{
		entities.emplace_back(std::make_unique<Walker>(sf::Vector2f(WALKER_WIDTH, WALKER_HEIGHT), texture, window.getSize()));
	}
}

void Game::checkInput()
{
	sf::Event event;
	while (window.pollEvent(event))
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

			sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);

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

	sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);

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
	window.draw(spriteWorld);
	window.display();
}

void Game::drawGrid()
{
	for (int i = 0; i < WINDOW_WIDTH; ++i)
	{
		if ((i % WALKER_WIDTH) == 0)
		{
			float ifL = static_cast<float>(i);
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(ifL, 0.f), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f)),
				sf::Vertex(sf::Vector2f(ifL, static_cast<float>(WINDOW_HEIGHT)), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f))
			};
			myRenderTexture.draw(line, 2, sf::Lines);
		}
	}
	for (int i = 0; i < WINDOW_HEIGHT; ++i)
	{
		if ((i % WALKER_HEIGHT) == 0)
		{
			float ifL = static_cast<float>(i);
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(0.f, ifL), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f)),
				sf::Vertex(sf::Vector2f(static_cast<float>(WINDOW_WIDTH), ifL), sf::Color(95, 95, 95), sf::Vector2f(100.f, 100.f))
			};
			myRenderTexture.draw(line, 2, sf::Lines);
		}
	}
}


void Game::checkCollisions(){
	for (int a = 0; a < WALKER_COUNT; ++a){
		for (int b = 0; b < WALKER_COUNT; ++b){
			if (a != b)
			{
				entities[a]->checkCollision(entities[b]->getBounds());
			}
		}
	}
}
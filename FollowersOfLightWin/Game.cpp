#include "Game.h"

Game::Game() : windowWidth(1024), windowHeight(768), walkerCount(15), shaderFile("shader.frag"), running(false), detectionRadius(150.f), bpm(120)
{

	window.create(sf::VideoMode(windowWidth, windowHeight), "Followers!");

	window.setFramerateLimit(500);
	initializeWalkers();

	collector.setRadius(detectionRadius);

	//preload shader
	shader = ShaderLoader::getShader(shaderFile);
	shader->setParameter("frag_ScreenResolution", sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight)));
}

void Game::run()
{
	myRenderTexture.create(windowWidth, windowHeight);
	spriteWorld.setTexture(myRenderTexture.getTexture());
	spriteWorld.setOrigin(spriteWorld.getTextureRect().width / 2.f, spriteWorld.getTextureRect().height / 2.f);
	spriteWorld.setPosition(windowWidth / 2.f, windowHeight / 2.f);

	running = true;

	while (running)
	{

		clock.restart();

		window.clear(sf::Color::Black);
		myRenderTexture.clear();

		checkInput();
		update();
		draw();

		FrameTime ft = static_cast<float>(clock.getElapsedTime().asMilliseconds());

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
	for (int a = 0; a < walkerCount; ++a)
	{
		walkers.emplace_back(window.getSize().x / 2.f, window.getSize().y / 2.f, detectionRadius, bpm);
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
	}
}

void Game::update()
{
	currentSlice += lastFt;

	//update multiple times based on fps
	for (; currentSlice >= ftSlice; currentSlice -= ftSlice)
	{
		for (auto& walker : walkers)
		{
			walker.update(ftStep, window);
		}

		collector.update(window);
	}

	
}

void Game::draw()
{
	
	for (auto& walker : walkers)
	{

		walker.draw(myRenderTexture, spriteWorld, shader);
	}
	
	collector.draw(myRenderTexture);
	
	myRenderTexture.display();
	window.draw(spriteWorld);
	window.display();
}

#include "Game.h"

Game::Game() : windowWidth(1024), windowHeight(768), walkerCount(15), running(false), detectionRadius(80.f), collector(80.f)
{
	
	if (!texture.loadFromFile("../Walker.png"))
	{
		std::cout << "Texture error!" << std::endl;
	}
	
	//preload shader
	shader = ShaderLoader::getShader("shader.frag");
	shader->setParameter("frag_ScreenResolution", sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight)));
}

void Game::run()
{
	
	window.create(sf::VideoMode(windowWidth, windowHeight), "Followers!");
	window.setFramerateLimit(250);
	initializeWalkers();
	
	myRenderTexture.create(windowWidth, windowHeight);

	spriteWorld.setTexture(myRenderTexture.getTexture());
	spriteWorld.setOrigin(spriteWorld.getTextureRect().width / 2.f, spriteWorld.getTextureRect().height / 2.f);
	spriteWorld.setPosition(windowWidth / 2.f, windowHeight / 2.f);
	
	sf::Clock clock;

	running = true;

	while (running)
	{

		window.clear();
		myRenderTexture.clear(sf::Color(0, 0, 65));

		checkInput();
		update();
		checkCollisions();
		draw();

		sf::Int32 elapsedTime = clock.restart().asMilliseconds();
		FrameTime ft = static_cast<float>(elapsedTime);

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
		walkers.emplace_back(window.getSize(), detectionRadius, texture);
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
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);
				for (auto& walker : walkers){
					walker.checkSelect(mousePosition);
				}
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);
				for (auto& walker : walkers){
					walker.setTargetPosition(mousePosition);
				}
			}


		}
	}
}

void Game::checkCollisions(){
	for (int a = 0; a < walkerCount; ++a){
		for (int b = 0; b < walkerCount; ++b){
			if (a != b)
			{
				walkers[a].checkCollision(walkers[b]);
			}
		}
	}
}

void Game::update()
{
	currentSlice += lastFt;

	sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);

	//update multiple times based on fps
	for (; currentSlice >= ftSlice; currentSlice -= ftSlice)
	{
		for (auto& walker : walkers)
		{
			walker.update(ftStep, window.getSize(), mousePosition);
		}

		collector.update(mousePosition);
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

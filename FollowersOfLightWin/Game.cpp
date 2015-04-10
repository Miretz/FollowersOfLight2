#include "Game.h"

Game::Game() : windowWidth(800), windowHeight(600), walkerCount(15), shaderFile("shader.frag"), running(false)
{

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	
	window.create(sf::VideoMode(windowWidth, windowHeight), "Followers!", sf::Style::Default, settings);

	window.setFramerateLimit(500);
	initializeWalkers();

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

		auto time1(std::chrono::high_resolution_clock::now());

		window.clear(sf::Color::Black);
		myRenderTexture.clear();

		checkInput();
		update();
		draw();

		auto time2(std::chrono::high_resolution_clock::now());
		auto elapsedTime(time2 - time1);
		FrameTime ft{ std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(elapsedTime).count() };

		lastFt = ft;

		auto ftSeconds(ft / 1000.f);
		if (ftSeconds > 0.f)
		{
			auto fps(1.f / ftSeconds);
			window.setTitle("FT: " + std::to_string(ft) + "\tFPS: " + std::to_string(fps));
		}
	}
}

void Game::initializeWalkers()
{
	for (int a = 0; a < walkerCount; ++a)
	{
		walkers.emplace_back(window.getSize().x / 2.f, window.getSize().y / 2.f);
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
	}
}

void Game::draw()
{
	//mouse circle
	sf::Vector2f mousePosition = (sf::Vector2f) sf::Mouse::getPosition(window);
	sf::CircleShape shape(100);
	shape.setFillColor(sf::Color(0,0,0,25));
	shape.setOrigin(100, 100);
	shape.setPosition(mousePosition);

	shape.setOutlineThickness(10);
	shape.setOutlineColor(sf::Color(70, 70, 150, 85));

		
	for (int a = 0; a < walkerCount; ++a)
	{

		walkers[a].draw(myRenderTexture);

		shader->setParameter("frag_LightOrigin", walkers[a].getPosition());
		shader->setParameter("frag_LightColor", walkers[a].getColor());
		shader->setParameter("frag_LightAttenuation", walkers[a].getLightAttenuation());

		sf::RenderStates states;
		states.shader = shader;
		states.blendMode = sf::BlendAdd;

		myRenderTexture.draw(spriteWorld, states);

	}
	
	//draw mouse circle
	myRenderTexture.draw(shape);

	myRenderTexture.display();
	window.draw(spriteWorld);
	window.display();
}

#pragma once
#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WINDOW_TITLE "SFML Game by Miretz"

class WindowHandler : public sf::NonCopyable
{
public:

	static WindowHandler* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new WindowHandler();
		}
		return s_pInstance;
	}

	WindowHandler() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE){}

	sf::RenderWindow* getWindow()
	{
		return &window;
	}

private:

	static WindowHandler* s_pInstance;

	sf::RenderWindow window;

};

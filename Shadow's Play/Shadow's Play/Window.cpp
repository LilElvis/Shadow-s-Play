#include "Window.h"
#include "States.h"

namespace ENG
{
	//CREATE SINGLETON POINTER
	Window* Window::game = nullptr;

	Window::Window()
	{
		sf::RenderWindow window();
	}

	Window* Window::gameInstance()
	{
		if (game == nullptr)
		game = new Window();
		return game;
	}

	sf::RenderWindow* Window::GetSFMLWindow()
	{
		return &window;
	}

	//UPDATES WINDOW, OBJECTS, AND DRAWS EVERYTHING
	bool Window::check()
	{
		if (!window.isOpen()) return false;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return false;
			}
		}
		return true;
	}
}
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

	//ADDS OBJECTS TO LIST FOR UPDATES
	void Window::AddGameObject(GameObject* object)
	{
		objects.push_back(object);
	}

	//REMOVES OBJECTS FROM LIST
	void Window::RemoveGameObject(GameObject* object)
	{
		for (auto itr = objects.begin(), itrEnd = objects.end();
			itr != itrEnd; itr++)
		{
			if ((*itr) == object)
			{
				objects.erase(itr);
				std::cout << object->getName() << " Removed" << std::endl;
				return;
			}
		}
	}

	//UPDATES WINDOW, OBJECTS, AND DRAWS EVERYTHING
	bool Window::update(Mesh* meshPtr, Shader* shaderPtr, float t)
	{
		//ENG::Input::ResetKeys();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(47.0f / 255.0f, 47.0f / 255.0f, 47.0f / 255.0f, 0);

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

		//window.clear();

		for (auto itr = objects.begin(), itrEnd = objects.end();
			itr != itrEnd; itr++)
		{
			(*itr)->update(t);
		}

		for (auto itr = objects.begin(), itrEnd = objects.end();
			itr != itrEnd; itr++)
		{
			(*itr)->render(meshPtr, shaderPtr);
		}
		return true;
	}
}
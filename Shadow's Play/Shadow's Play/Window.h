#pragma once

#include "GL\glew.h"

#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include "SFML\Network.hpp"
#include "SFML\System.hpp"
#include "SFML\Window.hpp"
#include "SFML\OpenGL.hpp"

//LINKING LIBRARIES

#if _DEBUG
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#else
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-system.lib")
#endif

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "include/FMOD/fmod_vc.lib")

#include <vector>
#include <string>
#include <math.h>

#include "Input.h"
#include "GameObject.h"

namespace ENG
{
	//WINDOW MANAGEMENT AND UPDATE CLASS
	class Window
	{
	public:
		static Window* gameInstance();

		sf::RenderWindow* GetSFMLWindow();

		bool check();

	private:
		Window();
		static Window* game;

		sf::RenderWindow window;

		static bool initialized;
	};
}
#pragma once

#include "GL\glew.h"
#include <GL\GL.h>
#include <GL\GLU.h>
#include "SFML\Graphics\Texture.hpp"
#include <unordered_map>
#include <string>
#include <iostream>

namespace ENG
{
	//TEXTURE CLASS (WRAPPER FOR SFML TEXTURING)
	class Texture
	{
	public:
		static Texture* getTexPtr();
		std::unordered_map<std::string, Texture*> listOfTextures;

		bool LoadFromFile(std::string, std::string);
		void Unload();

		void Bind(GLenum);
		static void UnBind(GLenum);
	
		GLuint texHandle;
		unsigned int Width;
		unsigned int Height;

	private:
		Texture();
		static Texture* texPtr;
	};
}
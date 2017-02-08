#pragma once

#include "SFML\Graphics\Texture.hpp"
#include <unordered_map>
#include <string>

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
		sf::Texture getSFTexture();
	private:
		Texture();
		static Texture* texPtr;
		sf::Texture sfTexture;
	};
}
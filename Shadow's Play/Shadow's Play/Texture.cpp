#include "Texture.h"

namespace ENG
{
	Texture::Texture()
	{

	}

	//CREATE SINGLETON POINTER
	Texture* Texture::texPtr = nullptr;

	Texture* Texture::getTexPtr()
	{
		if (texPtr == nullptr)
			texPtr = new Texture();
		return texPtr;
	}

	//LOAD TEXTURES FROM PNG FILES
	bool Texture::LoadFromFile(std::string name, std::string aFilePath)
	{
		Texture* tempTexture = new Texture();
		tempTexture->sfTexture.loadFromFile(aFilePath);

		listOfTextures[name] = tempTexture;

		return true;
	}

	//UNLOAD TEXTURES
	void Texture::Unload()
	{
		sfTexture.~Texture();
	}

	//HANDLE FOR SF TEXTURE
	sf::Texture Texture::getSFTexture()
	{
		return sfTexture;
	}
}
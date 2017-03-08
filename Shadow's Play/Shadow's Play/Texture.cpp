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

		sf::Image texture;

		texture.loadFromFile(aFilePath);
		texture.flipVertically();

		Width = texture.getSize().x;
		Height = texture.getSize().y;

		glGenTextures(1, &texHandle);
		glBindTexture(GL_TEXTURE_2D, texHandle);

		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr());

		if (texHandle == 0)
		{
			std::cout << "\nTexture's fucked yo.\n";
		}

		glBindTexture(GL_TEXTURE_2D, texHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		tempTexture->texHandle = texHandle;
		tempTexture->Width = Width;
		tempTexture->Height = Height;

		listOfTextures[name] = tempTexture;

		return true;
	}

	//UNLOAD TEXTURES
	void Texture::Unload()
	{
		for (int i = 0; i < listOfTextures.size(); ++i)
		{
			for (auto itr = listOfTextures.begin(), itrEnd = listOfTextures.end(); itr != itrEnd; itr++)
			{

				glDeleteTextures(1, &(*itr).second->texHandle);
			}
		}
	}

	void Texture::Bind(GLenum textureSlot)
	{
		glActiveTexture(textureSlot);
		glBindTexture(GL_TEXTURE_2D, texHandle);
	}

	void Texture::UnBind(GLenum textureSlot)
	{
		glActiveTexture(textureSlot);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}
}
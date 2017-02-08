#pragma once
#include "GameObject.h"

namespace ENG
{
	//INHERITED SCENERY OBJECT CLASS
	class SceneObject : public GameObject
	{
	public:
		SceneObject(std::string, GLuint, sf::Texture);
		void update(float);
		void reset();
	private:
	};
}
//Cameron van Velzen - 100591663
#pragma once
#include "GameObject.h"

namespace ENG
{
	//INHERITED SCENERY OBJECT CLASS
	class SceneObject : public GameObject
	{
	public:
		SceneObject(std::string, GLuint, Texture, Texture, Texture, Texture, GLuint);
		void update(float, float);
		void reset();
	private:
	};
}
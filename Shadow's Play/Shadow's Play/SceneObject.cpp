#include "SceneObject.h"

namespace ENG
{
	SceneObject::SceneObject(std::string name, GLuint aVAO, sf::Texture aTexture) : GameObject(name, aVAO, aTexture)
	{

	}

	//UPDATES EACH FRAME
	//*WILL BE USEFUL ONCE PUSHING OBJECTS ARE IMPLEMENTED
	void SceneObject::update(float t)
	{
		Input input;
	}

	//RESETS NECESSARY VARIABLES PER LEVEL
	void SceneObject::reset()
	{
		willWin = false;
		willLose = false;

		transform.zeroMatrix();

		uInvisible = 1.0f;
		uDiffuseAdd = glm::vec3(0.0f);
		uDiffuseMult = glm::vec3(1.0f);
		uAmbientAdd = glm::vec3(0.25f);
		uAmbientMult = glm::vec3(1.0f);
		uEmissiveAdd = glm::vec3(0.0f);
		uEmissiveMult = glm::vec3(1.0f);

		velocity = glm::vec3(0.0f);
	}
}
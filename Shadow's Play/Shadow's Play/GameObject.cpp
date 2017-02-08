#include "GameObject.h"

namespace ENG
{
	//CONSTRUCTS OBJECTS AND INITIALIZES VARIABLES
	GameObject::GameObject(std::string name, GLuint aVAO, sf::Texture aTexture) : objectName(name), renderable(aVAO), texture(aTexture)
	{
		transform = Transform();
		BBBL = glm::vec2(-1.0f, 1.0f);
		BBFR = glm::vec2(1.0f, -1.0f);
		
		willWin = false;
		willLose = false;

		uInvisible = 1.0f;
		uDiffuseAdd = glm::vec3(0.0f);
		uDiffuseMult = glm::vec3(1.0f);
		uAmbientAdd = glm::vec3(0.25f);
		uAmbientMult = glm::vec3(1.0f);
		uEmissiveAdd = glm::vec3(0.0f);
		uEmissiveMult = glm::vec3(1.0f);

		velocity = glm::vec3(0.0f);
	}

	//VIRTUAL FUNCTION NEEDS NO DEFINITION IN THE PARENT
	void GameObject::update(float t)
	{
		
	}

	//RENDERS THE RENDERABLES
	void GameObject::render(Mesh* meshPtr, Shader* shaderPtr)
	{
			transform.setMatrix(glm::mat4(
			transform.getMatrix()[0][0], transform.getMatrix()[0][1], transform.getMatrix()[0][2], position.x,
			transform.getMatrix()[1][0], transform.getMatrix()[1][1], transform.getMatrix()[1][2], position.y,
			transform.getMatrix()[2][0], transform.getMatrix()[2][1], transform.getMatrix()[2][2], position.z,
				0.0f, 0.0f, 0.0f, 1.0f));

		glBindVertexArray(renderable);

		shaderPtr->sendUniformMat4("uModel", getTransform()->getFirstElement(), true);
		shaderPtr->sendUniform("uDiffuseAdd", uDiffuseAdd);
		shaderPtr->sendUniform("uDiffuseMult", uDiffuseMult);
		shaderPtr->sendUniform("uAmbientAdd", uAmbientAdd);
		shaderPtr->sendUniform("uAmbientMult", uAmbientMult);
		shaderPtr->sendUniform("uEmissiveAdd", uEmissiveAdd);
		shaderPtr->sendUniform("uEmissiveMult", uEmissiveMult);
		shaderPtr->sendUniform("uInvisible", uInvisible);
		
		sf::Texture::bind(&texture);

		glDrawArrays(GL_TRIANGLES, 0, (meshPtr->listOfMeshes[objectName]->getNumberOfVerticies()));
		glBindVertexArray(0);
	}

	//ACCESSORS & MUTATORS
	Transform* GameObject::getTransform()
	{
		return &transform;
	}

	GLuint GameObject::getRenderable()
	{
		return renderable;
	}

	void GameObject::setPosition(glm::vec3 posIn)
	{
		position = glm::vec3(posIn.x, posIn.y, posIn.z);
	}

	void GameObject::addAnimation(std::string name, GLuint targetToAdd)
	{
		animationTargets[name] = targetToAdd;
	}

	GLuint GameObject::getAnimation(std::string name)
	{
		return animationTargets[name];
	}

	std::string GameObject::getName()
	{
		return objectName;
	}

	glm::vec3 GameObject::getPosition()
	{
		return position;
	}

	void GameObject::setLastPosition(glm::vec3 posIn)
	{
		lastPosition = glm::vec3(posIn.x, posIn.y, posIn.z);
	}

	glm::vec3 GameObject::getLastPosition()
	{
		return lastPosition;
	}

	void GameObject::setWin(bool isWin)
	{
		willWin = isWin;
	}

	bool GameObject::getWin()
	{
		return willWin;
	}

	void GameObject::setLoss(bool isLoss)
	{
		willLose = isLoss;
	}

	bool GameObject::getLoss()
	{
		return willLose;
	}
}

	

	
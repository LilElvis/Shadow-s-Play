#include "GameObject.h"

namespace ENG
{
	//CONSTRUCTS OBJECTS AND INITIALIZES VARIABLES
	GameObject::GameObject(std::string name, GLuint aVAO, Texture anAlbedo, Texture aNormal, Texture aSpecular, Texture anEmissive, GLuint layerToDrawOn) : objectName(name), renderable(aVAO), albedo(anAlbedo), normal(aNormal), specular(aSpecular), emissive(anEmissive)
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

		layerNumber = layerToDrawOn;

		velocity = glm::vec3(0.0f);
	}

	//VIRTUAL FUNCTION NEEDS NO DEFINITION IN THE PARENT
	void GameObject::update(float t, float totalTime)
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
		shaderPtr->sendUniform("uSpecularAdd", uSpecularAdd);
		shaderPtr->sendUniform("uSpecularMult", uSpecularMult);
		shaderPtr->sendUniform("uEmissiveAdd", uEmissiveAdd);
		shaderPtr->sendUniform("uEmissiveMult", uEmissiveMult);
		shaderPtr->sendUniform("uInvisible", uInvisible);

		shaderPtr->sendUniform("uvOffset", uUVOffset);
		
		albedo.Bind(GL_TEXTURE0);
		normal.Bind(GL_TEXTURE1);
		specular.Bind(GL_TEXTURE2);
		emissive.Bind(GL_TEXTURE3);

		glDrawArrays(GL_TRIANGLES, 0, (meshPtr->listOfMeshes[objectName]->getNumberOfVerticies()));
		glBindVertexArray(0);

		emissive.UnBind(GL_TEXTURE3);
		specular.UnBind(GL_TEXTURE2);
		normal.UnBind(GL_TEXTURE1);
		albedo.UnBind(GL_TEXTURE0);
	}

	void GameObject::drawTransparent(Mesh* meshPtr,Shader* shaderPtr )
	{
		transform.setMatrix(glm::mat4(
			transform.getMatrix()[0][0], transform.getMatrix()[0][1], transform.getMatrix()[0][2], position.x,
			transform.getMatrix()[1][0], transform.getMatrix()[1][1], transform.getMatrix()[1][2], position.y,
			transform.getMatrix()[2][0], transform.getMatrix()[2][1], transform.getMatrix()[2][2], position.z,
			0.0f, 0.0f, 0.0f, 1.0f));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(renderable);

		shaderPtr->sendUniformMat4("uModel", getTransform()->getFirstElement(), true);
		shaderPtr->sendUniform("uDiffuseAdd", uDiffuseAdd);
		shaderPtr->sendUniform("uDiffuseMult", uDiffuseMult);
		shaderPtr->sendUniform("uAmbientAdd", uAmbientAdd);
		shaderPtr->sendUniform("uAmbientMult", uAmbientMult);
		shaderPtr->sendUniform("uEmissiveAdd", uEmissiveAdd);
		shaderPtr->sendUniform("uEmissiveMult", uEmissiveMult);
		shaderPtr->sendUniform("uInvisible", uInvisible);

		albedo.Bind(GL_TEXTURE0);

		glDrawArrays(GL_TRIANGLES, 0, (meshPtr->getNumberOfVerticies()));
		glBindVertexArray(0);

		albedo.UnBind(GL_TEXTURE0);

		glDisable(GL_BLEND);
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

	glm::vec3 GameObject::getVelocity()
	{
		return velocity;
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

	

	
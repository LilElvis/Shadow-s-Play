#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Input.h"
#include "Texture.h"

namespace ENG
{
	//GAME OBJECT PARENT CLASS
	class GameObject
	{
	public:
		GameObject(std::string, GLuint, Texture, Texture, Texture, Texture, GLuint);
		
		//UTILITY FUNCTIONS
		void render(Mesh*, Shader*);
		void drawTransparent(Mesh*, Shader*);
		virtual void update(float, float) = 0;
		virtual void reset() = 0;
		Transform* getTransform();
		GLuint getRenderable();

		glm::vec3 getVelocity();
		void setPosition(glm::vec3);
		void addAnimation(std::string, GLuint);
		GLuint getAnimation(std::string);
		std::string getName();
		glm::vec3 getPosition();
		glm::vec2 BBBL;
		glm::vec2 BBFR;
		void setWin(bool);
		bool getWin();
		void setLoss(bool);
		bool getLoss();

		//SHADER UNIFORMS
		float uInvisible;
		glm::vec3 uDiffuseAdd;
		glm::vec3 uDiffuseMult;
		glm::vec3 uAmbientAdd;
		glm::vec3 uAmbientMult;
		glm::vec3 uSpecularAdd;
		glm::vec3 uSpecularMult;
		glm::vec3 uEmissiveAdd;
		glm::vec3 uEmissiveMult;
		glm::vec2 uUVOffset;

		std::vector<glm::vec2> UVOffsets;

	protected:
		glm::vec3 acceleration;
		glm::vec3 velocity;
		void setLastPosition(glm::vec3);
		glm::vec3 getLastPosition();
		Transform transform;
		bool willWin;
		bool willLose;
		std::unordered_map<std::string, GLuint> animationTargets;
		GLuint layerNumber;
		
	private:
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 lastPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		const GLuint renderable;
		std::string objectName; //WARNING: MUST BE IDENTICAL TO THE NAME IN ALL LISTS
		Texture albedo;
		Texture normal;
		Texture specular;
		Texture emissive;
	};
}
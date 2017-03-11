#pragma once

#include "Window.h"
#include "GameObject.h"

namespace ENG
{
	//INHERITED PLAYER CLASS
	class Player : public GameObject
	{
	public:
		Player(std::string, GLuint, Texture, Texture, Texture, Texture, GLuint);
		glm::vec3 NyxSeekPoint(glm::vec3, glm::vec3, float);
		void movement(float, float, float);
		void speed();
		void update(float);
		bool collisionCheck(std::vector<GameObject*>);
		void setStartPosition(glm::vec3);
		glm::vec3 getStartPosition();
		void reset();
		void setLifeLost(bool);
		void setNyxPaused(bool);
		bool getLifeLost();
		bool getIsDead();
		bool getNyxPaused();
		float last_y_rotate = 0;

	private:
		float timeSinceStart;
		glm::vec3 startingPosition;
		bool colliding = false;
		int lives;
		bool lifeLost;
		bool isDead;
		bool paused;
		glm::vec3 seekPoint = glm::vec3(0.1f, 0.0f, 0.1f);
		glm::vec3 forwards;

		Input input;
		float appliedAcceleration = 40.0f;
		float maxVelocity = 30.0f;
		float nyxSpeed = 0.1f;
	};
}
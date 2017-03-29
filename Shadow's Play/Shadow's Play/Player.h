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
		void update(float, float);
		void dash(float);
		bool getDashed();
		bool collisionCheck(std::vector<GameObject*>);
		void setStartPosition(glm::vec3);
		glm::vec3 getStartPosition();
		void reset();
		void setLifeLost(bool);
		void setNyxPaused(bool);
		bool getLifeLost();
		bool getIsDead();
		bool getNyxPaused();
		int getNumOfLives();
		float last_y_rotate = 0;
		float timeOfLastDash;

	private:
		float timeSinceStart;
		glm::vec3 startingPosition;
		bool colliding = false;
		int lives;
		bool lifeLost;
		bool isDead;
		bool paused;
		glm::vec3 seekPoint = glm::vec3(0.1f, 0.0f, 0.1f);
		float dashCooldown = 3.0f;
		bool hasDashed = false;
		glm::vec3 collideDistanceX = glm::vec3(0.25f, 0.0f, 0.0f);
		glm::vec3 collideDistanceZ = glm::vec3(0.0f, 0.0f, 0.25f);

		Input input;
		float appliedAcceleration = 40.0f;
		float maxVelocity = 30.0f;
		float speedMult = 0.15f;
	};
}
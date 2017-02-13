#pragma once

#include "GameObject.h"

namespace ENG
{
	//INHERITED PLAYER CLASS
	class Player : public GameObject
	{
	public:
		Player(std::string, GLuint, Texture, Texture, Texture, Texture, GLuint);
		void update(float);
		bool collisionCheck(std::vector<GameObject*>);
		void setStartPosition(glm::vec3);
		glm::vec3 getStartPosition();
		void reset();
		void setLifeLost(bool);
		bool getLifeLost();
		bool getIsDead();
	private:
		float timeSinceStart;
		glm::vec3 startingPosition;
		bool colliding = false;
		int lives;
		bool lifeLost;
		bool isDead;

		Input input;
		float appliedAcceleration = 30.0f;
		float maxVelocity = 20.0f;
	};
}
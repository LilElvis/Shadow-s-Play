#include "Player.h"
#include "AnimationMath.h"

namespace ENG
{
	Player::Player(std::string name, GLuint aVAO, Texture anAlbedo, Texture aNormal, Texture aSpecular, Texture anEmissive, GLuint layerToDrawOn) : GameObject(name, aVAO, anAlbedo, aNormal, aSpecular, anEmissive, layerToDrawOn)
	{
		colliding = false;
		lives = 2;
		isDead = false;
		lifeLost = false;
	}

	glm::vec3 Player::NyxSeekPoint(glm::vec3 moving, glm::vec3 toseek, float dist)
	{
		glm::vec3 newPosition(moving.x - toseek.x, 0.0f, moving.z - toseek.z);
		glm::vec3 forwardssss(0.0f);
		glm::normalize(newPosition);

		if (glm::length(newPosition) > dist)
		{
			newPosition.x *= dist;
			newPosition.z *= dist;
		}
		glm::vec3 normalVec = glm::normalize(newPosition);

		forwardssss.x = normalVec.x;
		forwardssss.z = normalVec.z;

		return newPosition;
	}

	float Player::angleVec(glm::vec3 vecOne, glm::vec3 vecTwo)
	{
		 return acosf(glm::dot(vecOne, vecTwo) / (glm::length(vecOne) * glm::length(vecTwo)));
	}

	void Player::update(float t)
	{
		//COLLISION CHECK
		if (colliding == true)
		{
			setPosition(getLastPosition());
		}
		else
		{
			setLastPosition(getPosition());
		}

		//MOVEMENT INPUT CHECK

		if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) < -20.0f && colliding == false) || ((input.GetKey(KeyCode::W) && colliding == false)) && !paused)
		{
			acceleration.z = -appliedAcceleration;
		}

		if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) > 20.0f && colliding == false) || ((input.GetKey(KeyCode::S) && colliding == false)) && !paused)
		{
			acceleration.z = appliedAcceleration;
		}

		if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 20.0f && colliding == false) || ((input.GetKey(KeyCode::D) && colliding == false)) && !paused)
		{
			acceleration.x = appliedAcceleration;
		}

		if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -20.0f && colliding == false) || ((input.GetKey(KeyCode::A) && colliding == false)) && !paused)
		{
			acceleration.x = -appliedAcceleration;
		}

		//TIME COUNT SINCE INPUT BEGAN
		if ((input.GetKey(ENG::KeyCode::W) || input.GetKey(ENG::KeyCode::S) || input.GetKey(ENG::KeyCode::A) || input.GetKey(ENG::KeyCode::D) || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) < -20.0f || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) > 20.0f || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 20.0f || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -20.0f) && !paused)
		{
			timeSinceStart += t;
		}
		else if(timeSinceStart > 0.0f)
		{ 
			timeSinceStart-= (t * 5.0f);
		}
			
		//UPDATE POSITION
		if (input.keyWasPressed)
		{
			velocity = (acceleration*timeSinceStart);
		
			//CAPS VELOCITY SO YOU CAN'T HIT SANIC SPEED
			if (velocity.x <= -maxVelocity)
				velocity.x = -maxVelocity;
			if (velocity.x >= maxVelocity)
				velocity.x = maxVelocity;

			if (velocity.z <= -maxVelocity)
				velocity.z = -maxVelocity;
			if (velocity.z >= maxVelocity)
				velocity.z = maxVelocity;
		
			seekPoint += (velocity * t) + (0.5f * acceleration * (t * t));
			clamp(getPosition(), getPosition() - 10.0f, getPosition() + 10.0f);
 			setPosition(glm::vec3(	getPosition().x - NyxSeekPoint(getPosition(), seekPoint, 1.0f).x, 
									getPosition().y + NyxSeekPoint(getPosition(), seekPoint, 1.0f).y,
									getPosition().z - NyxSeekPoint(getPosition(), seekPoint, 1.0f).z));
			transform.rotateY(angleVec(getLastPosition(), seekPoint));
			transform.zeroMatrix();
			std::cout << getPosition().x << " "<< getPosition().y << " " << getPosition().z << std::endl;
			std::cout << seekPoint.x << " " << seekPoint.y << " " << seekPoint.z << " " << std::endl;
		}

		acceleration = glm::vec3(0.0f);
		velocity = glm::vec3(0.0f);
		
		//INSURES TIME NEVER GOES NEGATIVE
		if (timeSinceStart <= 0.0f)
		{
			timeSinceStart = 0.0f;
		}
	}

	bool Player::collisionCheck(std::vector<GameObject*> otherOBJ)
	{
		//CHECKING AGAINST ANY OTHER OBJECTS BOUNDING BOX
		for (int i = 0; i < otherOBJ.size(); ++i)
		{
			float thisMinX = getPosition().x + BBBL.x;
			float thisMaxX = getPosition().x + BBFR.x;
			float thisMinZ = getPosition().z + BBFR.y;
			float thisMaxZ = getPosition().z + BBBL.y;
	
			float thatMinX = otherOBJ[i]->getPosition().x + otherOBJ[i]->BBBL.x;
			float thatMaxX = otherOBJ[i]->getPosition().x + otherOBJ[i]->BBFR.x;
			float thatMinZ = otherOBJ[i]->getPosition().z + otherOBJ[i]->BBFR.y;
			float thatMaxZ = otherOBJ[i]->getPosition().z + otherOBJ[i]->BBBL.y;
	
			if ((thisMinX >= thatMinX && thisMinX <= thatMaxX) ||
				(thisMaxX >= thatMinX && thisMaxX <= thatMaxX))
			{
				if ((thisMinZ >= thatMinZ && thisMinZ <= thatMaxZ) ||
					(thisMaxZ >= thatMinZ && thisMaxZ <= thatMaxZ))
				{
					if (otherOBJ[i]->getLoss() == true)
					{
						if (lives > 0)
						{
							lives -= 1;
							std::cout << "YOU LOST A LIFE!\n";
							setPosition(startingPosition);
							seekPoint = startingPosition;
							lifeLost = true;
						}
						else
						{
							std::cout << "YOU LOSE!!\n";
							setPosition(startingPosition);
							seekPoint = startingPosition;
							isDead = true;
						}
					}
					else
					{
						return colliding = true;
					}
				}
			}
		}
	return colliding = false;
	}

	//RESETS NECESSARY VARIABLES PER LEVEL
	void Player::reset()
	{
		setPosition(startingPosition);
		std::cout << "\nNYX RESET\n\n";
		lives = 2;
		isDead = false;
		lifeLost = false;
	}

	//ACCESSOR & MUTATOR FUNCTIONS
	void Player::setStartPosition(glm::vec3 startPosIn)
	{
		startingPosition = startPosIn;
	}

	glm::vec3 Player::getStartPosition()
	{
		return startingPosition;
	}

	void Player::setLifeLost(bool lifeLostIn)
	{
		lifeLost = lifeLostIn;
	}

	void Player::setNyxPaused(bool u_paused)
	{
		paused = u_paused;
	}

	bool Player::getLifeLost()
	{
		return lifeLost;
	}

	bool Player::getIsDead()
	{
		return isDead;
	}

	bool Player::getNyxPaused()
	{
		return paused;
	}
}
#include "Player.h"

namespace ENG
{
	Player::Player(std::string name, GLuint aVAO, Texture anAlbedo, Texture aNormal, Texture aSpecular, Texture anEmissive, GLuint layerToDrawOn) : GameObject(name, aVAO, anAlbedo, aNormal, aSpecular, anEmissive, layerToDrawOn)
	{
		colliding = false;
		lives = 2;
		isDead = false;
		lifeLost = false;
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

			transform.zeroMatrix();
			transform.rotateY(3.14159f);
		}

		if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) > 20.0f && colliding == false) || ((input.GetKey(KeyCode::S) && colliding == false)) && !paused)
		{
			acceleration.z = appliedAcceleration;

			transform.zeroMatrix();
			transform.rotateY(0.0f);
		}

		if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 20.0f && colliding == false) || ((input.GetKey(KeyCode::D) && colliding == false)) && !paused)
		{
			acceleration.x = appliedAcceleration;

			transform.zeroMatrix();
			transform.rotateY(1.57079f);
		}

		if ((sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -20.0f && colliding == false) || ((input.GetKey(KeyCode::A) && colliding == false)) && !paused)
		{
			acceleration.x = -appliedAcceleration;

			transform.zeroMatrix();
			transform.rotateY(-1.57079f);
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
		
			setPosition(getPosition() + (velocity * t) + (0.5f * acceleration * (t * t)));
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
							lifeLost = true;
						}
						else
						{
							std::cout << "YOU LOSE!!\n";
							setPosition(startingPosition);
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
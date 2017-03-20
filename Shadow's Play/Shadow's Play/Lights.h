#pragma once

#include "glm\matrix.hpp"
#include "glm\gtc\matrix_transform.hpp"

namespace ENG
{
	class PointLight
	{
	public:
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec4 color = glm::vec4(1.0f);
		float aConstant = 0.1;
		float aLinear = 0.1;
		float aQuadratic = 0.1875;
	};

	class SphereLight
	{
	public:
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec4 color = glm::vec4(1.0f);
		float aConstant = 0.1;
		float aLinear = 0.1;
		float aQuadratic = 0.1875;
	};
}
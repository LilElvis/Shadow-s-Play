//Cameron van Velzen - 100591663
#pragma once
#include "glm\common.hpp"
#include "glm\matrix.hpp"
#include <cmath>
#include "Input.h"

namespace ENG
{
	//HANDY TRANSFORM MATRIX CLASS
	class Transform
	{
	public:
		Transform();

		glm::mat4 getMatrix();
		void setMatrix(glm::mat4);
		float * getFirstElement();
		glm::mat4 rotateX(const float);
		glm::mat4 rotateY(const float);
		glm::mat4 rotateZ(const float);
		glm::mat4 translate(const float, const float, const float);
		void zeroMatrix();
	private:
		glm::mat4 matrix = glm::mat4();

	};
}
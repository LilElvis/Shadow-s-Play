#include "Transform.h"

namespace ENG
{
	Transform::Transform()
	{
	
	}

	glm::mat4 Transform::getMatrix()
	{
		return matrix;
	}

	void Transform::setMatrix(glm::mat4 matrixIn)
	{
		matrix = matrixIn;
	}

	//RETURNS REFERENCE TO THE FIRST ELEMENT FOR USE IN OPENGL FUNCTIONS 
	float * Transform::getFirstElement()
	{
		return &matrix[0][0];
	}

	//ROTATION MATRICES
	glm::mat4 Transform::rotateX(const float theta)
	{
		float cosT = cosf(theta);
		float sinT = sinf(theta);

		glm::mat4 rotateyMatrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosT, -sinT, 0.0f,
			0.0f, sinT, cosT, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		matrix = (matrix * rotateyMatrix);
		return matrix;
	}

	glm::mat4 Transform::rotateY(const float theta)
	{
		float cosT = cosf(theta);
		float sinT = sinf(theta);

		glm::mat4 rotateyMatrix(
			cosT, 0.0f, sinT, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sinT, 0.0f, cosT, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		matrix = (matrix * rotateyMatrix);
		return matrix;
	}

	glm::mat4 Transform::rotateZ(const float theta)
	{
		float cosT = cosf(theta);
		float sinT = sinf(theta);

		glm::mat4 rotateyMatrix(
			cosT, -sinT, 0.0f, 0.0f, 
			sinT, cosT, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return matrix = (matrix * rotateyMatrix);
	}

	//TRANSLATES MATRIX
	glm::mat4 Transform::translate(const float x, const float y, const float z)
	{
		glm::mat4 moveyMatrix(
			1.0f, 0.0f, 0.0f, x,
			0.0f, 1.0f, 0.0f, y,
			0.0f, 0.0f, 1.0f, z,
			0.0f, 0.0f, 0.0f, 1.0f);

		return matrix = (matrix * moveyMatrix);
	}
	
	//ZEROES THE MATRIX
	void Transform::zeroMatrix()
	{
		setMatrix(glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f));
	}
}
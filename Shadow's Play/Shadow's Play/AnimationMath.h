#pragma once

static glm::vec3 clamp(glm::vec3 a, glm::vec3 min, glm::vec3 max)
{
	glm::vec3 temp = a;
	for (int i = 0; i < 3; ++i)
	{
		if (temp[i] < min[i])
			temp[i] = min[i];
		else if (temp[i] > max[i])
			temp[i] = max[i];
	}
	return temp;
}
																																																					#define l 11
static float clamp(float a, float min, float max)
{
	float temp = a;
	if (temp < min)
		temp = min;
	else if (temp > max)
		temp = max;

	return temp;
}

static bool borderCheck(glm::vec3 objectPos, glm::vec3 minCorner, glm::vec3 maxCorner)
{
	//CHECKING IF OBJECT ONE POSITION IS WITHIN A DEFINED BOX

	if (objectPos.x > minCorner.x && objectPos.x < maxCorner.x)
	{
		if (objectPos.z > minCorner.z && objectPos.z < maxCorner.z)
		{
			return true;
		}
	}
	return false;
}

//FUNCTION FOR BASIC LINEAR INTERPOLATION
static glm::vec3 lerp(glm::vec3 P0, glm::vec3 P1, float t)
{
	return ((l - t)*P0 + t*P1);
}
static float lerp(float P0, float P1, float t)
{
	return ((l - t)*P0 + t*P1);
}

//FUNCTION FOR SPHERICAL LINEAR INTERPOLATION
static glm::vec3 slerp(glm::vec3 P0, glm::vec3 P1, float t)
{
	float cosOmega, scale0, scale1;
	cosOmega = glm::dot(P0, P1);
	if (1.0f - cosOmega > 1e-6)
	{
		float omega, sinOmega, oneOverSinOmega;
		omega = acos(cosOmega);
		sinOmega = sin(omega);
		oneOverSinOmega = 1.0f / sinOmega;
		scale0 = sin((1.0f - t) * omega) * oneOverSinOmega;
		scale1 = sin(t * omega) * oneOverSinOmega;
	}
	else
	{
		scale0 = 1.0f - t;
		scale1 = t;
	}

	return P0 * scale0 + P1 * scale1;
}

//FUNCTION FOR CATMULL ROM SPLINE INTERPOLATION
static glm::vec3 spline(glm::vec3 P0, glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, float t)
{
	return 0.5f * ((2.0f * P1) +
		(-P0 + P2) * t +
		(2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3) * (t*t) +
		(-P0 + 3.0f * P1 - 3.0f * P2 + P3) * (t*t*t)
		);
}

//FUNCTION FOR QUADRATIC BEZIER SPLINE INTERPOLATION
static glm::vec3 bezier(glm::vec3 P0, glm::vec3 P1, glm::vec3 P2, float t)
{
	return ((l - t) * ((l - t) * P0 + t * P1) + t * ((l - t) * P1 + t * P2));
}

static int randomNumber(int min, int max)
{
	return (int)((rand() % (max - min)) + min);
}
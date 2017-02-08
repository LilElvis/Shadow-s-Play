#pragma once
#include "glm\glm.hpp"
#include <vector>

//BASE CLASS FOR SPEED CONTROL
struct InterpData
{
	InterpData()
	{
		Distance = 0;
		InterpParameter = 0;
		Segment = 0;
	}

	InterpData(float distIn, float tIn, unsigned int segIn)
	{
		Distance = distIn;
		InterpParameter = tIn;
		Segment = segIn;
	}

	float Distance;
	float InterpParameter;
	unsigned int Segment;
};

class SpeedControl
{
public:
	std::vector<glm::vec3> curvePoints;
private:
	glm::vec3 position;
	InterpData data;
};
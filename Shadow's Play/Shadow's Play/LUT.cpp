#include "LUT.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "GLM/glm.hpp"

ENG::LUT::LUT()
{
	handle = 0;
	dataSize = 0;
}

ENG::LUT::~LUT()
{
	glDeleteTextures(1, &handle);

	handle = 0;
}

void ENG::LUT::loadData(std::string filename)
{
	std::ifstream inFile(filename);

	if (!inFile)		//output error message if the file doesnt exist in the file path
	{
		std::cout << "ERROR: The LUT at path ( " << filename << " ) could not be found." << std::endl;
		abort();
	}
	std::vector <glm::vec3> texData;	//vector to hold texture data

	char buffer[256];					//max size one line will be (wont really ever reach it)

	glm::vec3 nextDataPoint;			//vec3 to store the data point (rgb values)

	bool inHeader = true;				//check to see if still in the header

	while (inFile.getline(buffer, 256))
	{
		if (inHeader)
		{
			if (strstr(buffer, "LUT_3D_SIZE"))
			{
				std::string temp = buffer;

				temp = temp.substr(12, std::string::npos);

				dataSize = std::stoi(temp);

				inHeader = false;
			}
		}

		else
		{
			sscanf_s(buffer, "%f %f %f", &nextDataPoint.x, &nextDataPoint.y, &nextDataPoint.z);		//scans line by line and records the float points into nextDataPoint

			texData.push_back(nextDataPoint);		//adds it to the vector (list)
		}
	}

	inFile.close();

	glGenTextures(1, &handle);

	glBindTexture(GL_TEXTURE_3D, handle);		

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//minification
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//magnification

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//x axis wrap
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//y axis wrap
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	//z axis wrap

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, dataSize, dataSize, dataSize, 0, GL_RGB, GL_FLOAT, &texData[0]);	//creates the texture of lut

	glBindTexture(GL_TEXTURE_3D, NULL);			//binds the lut to the texture
	texData.clear();
}					

void ENG::LUT::bind(GLenum textureunit)			//binds the texture
{
	glActiveTexture(textureunit);
	glBindTexture(GL_TEXTURE_3D, handle);
}
#pragma once

#include "GL\glew.h"
#include "SFML\OpenGL.hpp"
#include "SFML\System\Vector3.hpp"
#include "SFML\System\Vector2.hpp"
#include <fstream>
#include <iostream>
#include <vector>



namespace ENG
{
	//MESH LOADER CLASS
	class PlayerMesh
	{
	public:
		bool LoadFromFile(const std::string, const std::string &, const std::string &);

		unsigned int getNumberOfVerticies();

		GLuint VAO;
		GLuint VBO_VerticiesA;
		GLuint VBO_NormalsA;
		GLuint VBO_UVs;
		GLuint VBO_Tangents;
		GLuint VBO_BiTangents;
		GLuint VBO_VerticiesB;
		GLuint VBO_NormalsB;

		std::vector<float> verticesA;
		std::vector<float> normalsA;
		std::vector<float> verticesB;
		std::vector<float> normalsB;

	private:

		unsigned int NumberOfFaces;
		unsigned int NumberOfVerticies;
	};
}
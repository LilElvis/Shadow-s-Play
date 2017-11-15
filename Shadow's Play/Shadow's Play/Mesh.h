//Cameron van Velzen - 100591663
#pragma once

#include "GL\glew.h"
#include <unordered_map>
#include "SFML\OpenGL.hpp"
#include "SFML\System\Vector3.hpp"
#include "SFML\System\Vector2.hpp"
#include <fstream>
#include <iostream>



namespace ENG
{
	//MESH LOADER CLASS
	class Mesh
	{
	public:
		static Mesh* getMeshPtr();
		std::unordered_map<std::string, Mesh*> listOfMeshes;

			bool LoadFromFile(const std::string, const std::string &);

			void Unload();

			unsigned int getNumberOfVerticies();

			GLuint VBO_Verticies;
			GLuint VBO_UVs;
			GLuint VBO_Normals;
			GLuint VBO_Tangents;
			GLuint VBO_BiTangents;
			GLuint VAO;
	private:
		Mesh();
		static Mesh* meshPtr;

			unsigned int NumberOfFaces;
			unsigned int NumberOfVerticies;
	};
}
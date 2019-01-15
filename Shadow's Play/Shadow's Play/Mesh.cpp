#include "Mesh.h"
#include "glm\common.hpp"
#include "glm\matrix.hpp"

#define CHAR_BUFFER_SIZE 128
#define BUFFER_OFFSET(i) ((char *)0 + (i))

namespace ENG
{
	Mesh::Mesh()
	{

	}
	
	//CREATE SINGLETON POINTER
	Mesh* Mesh::meshPtr = nullptr;

	//TEMPORARY CONTAINER FOR FACE DATA
	struct MeshFace
	{
		MeshFace()
		{
			verticies[0] = 0;
			verticies[1] = 0;
			verticies[2] = 0;

			textureUVs[0] = 0;
			textureUVs[1] = 0;
			textureUVs[2] = 0;

			normals[0] = 0;
			normals[1] = 0;
			normals[2] = 0;
		}

		MeshFace(unsigned int vert1, unsigned int vert2, unsigned int vert3,
			unsigned int UV1, unsigned int UV2, unsigned int UV3,
			unsigned int norm1, unsigned int norm2, unsigned int norm3)
		{
			verticies[0] = vert1;
			verticies[1] = vert2;
			verticies[2] = vert3;

			textureUVs[0] = UV1;
			textureUVs[1] = UV2;
			textureUVs[2] = UV3;

			normals[0] = norm1;
			normals[1] = norm2;
			normals[2] = norm3;
		}

		unsigned verticies[3];
		unsigned textureUVs[3];
		unsigned normals[3];
	};

	//LOADS DATA FROM OBJ FORMAT FILES
	bool Mesh::LoadFromFile(const std::string name, const std::string &file)
	{
		//CHECKS TO SEE IF THE MESH IS ALREADY IN THE LIST
		if (listOfMeshes.count(name) > 0) return true;

		std::ifstream input;

		input.open(file);

		if (!input)
		{
			std::cout << "Could not open the file." << std::endl;
			return false;
		}

		char inputString[CHAR_BUFFER_SIZE];

		//PACKED DATA
		std::vector<glm::vec3> vertexData;
		std::vector<glm::vec2> textureData;
		std::vector<glm::vec3> normalData;

		std::vector<MeshFace> faceData;

		//UNPACKED DATA
		std::vector<float> unPackedVertexData;
		std::vector<float> unPackedTextureData;
		std::vector<float> unPackedNormalData;
		std::vector<float> calculatedTangentData;
		std::vector<float> calculatedBiTangentData;

		//READ/RECORD THE DATA
		while (!input.eof())
		{
			input.getline(inputString, CHAR_BUFFER_SIZE);

			if (std::strstr(inputString, "#") != nullptr)
			{
				//THIS LINE IS A COMMENT
				continue;
			}
			else if (std::strstr(inputString, "vn") != nullptr)
			{
				//THIS LINE HAS NORMAL DATA
				glm::vec3 temp;

				std::sscanf(inputString, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
				normalData.push_back(temp);
			}
			else if (std::strstr(inputString, "vt") != nullptr)
			{
				//THIS LINE HAS UV DATA
				glm::vec2 temp;

				std::sscanf(inputString, "vt %f %f", &temp.x, &temp.y);
				textureData.push_back(temp);
			}
			else if (std::strstr(inputString, "v") != nullptr)
			{
				//THIS LINE HAS VERTEX DATA
				glm::vec3 temp;

				std::sscanf(inputString, "v %f %f %f", &temp.x, &temp.y, &temp.z);
				vertexData.push_back(temp);
			}
			else if (strstr(inputString, "s off") != nullptr)
			{
				//DO NOTHING
			}
			else if (inputString[0] == 'f')
			{
				MeshFace temp;

				std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
					&temp.verticies[0], &temp.textureUVs[0], &temp.normals[0],
					&temp.verticies[1], &temp.textureUVs[1], &temp.normals[1],
					&temp.verticies[2], &temp.textureUVs[2], &temp.normals[2]);

				faceData.push_back(temp);
			}
		}

		input.close();
		
		//UNPACK THE DATA
		for (unsigned i = 0; i < faceData.size(); i++)
		{
			glm::vec3 vertexA, vertexB, vertexC;
			glm::vec2 uvA, uvB, uvC;

			vertexA = vertexData[faceData[i].verticies[0] - 1];
			vertexB = vertexData[faceData[i].verticies[1] - 1];
			vertexC = vertexData[faceData[i].verticies[2] - 1];

			uvA = textureData[faceData[i].textureUVs[0] - 1];
			uvB = textureData[faceData[i].textureUVs[1] - 1];
			uvC = textureData[faceData[i].textureUVs[2] - 1];

			glm::vec3 deltaPosA, deltaPosB;
			glm::vec2 deltaUVA, deltaUVB;

			deltaPosA = vertexB - vertexA;
			deltaPosB = vertexC - vertexA;

			deltaUVA = uvB - uvA;
			deltaUVB = uvC - uvA;

			float f = 1.0f/(deltaUVA.x * deltaUVB.y - deltaUVA.y * deltaUVB.x);
			glm::vec3 tangent, biTangent;
			tangent = f * (deltaPosA * deltaUVB.y - deltaPosB * deltaUVA.y);
			biTangent = f * (deltaPosB * deltaUVA.x - deltaPosA * deltaUVB.x);

			for (unsigned j = 0; j < 3; j++)
			{
				unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].x);
				unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].y);
				unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].z);

				unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
				unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);

				unPackedVertexData.push_back(vertexData[faceData[i].verticies[j] - 1].x);
				unPackedVertexData.push_back(vertexData[faceData[i].verticies[j] - 1].y);
				unPackedVertexData.push_back(vertexData[faceData[i].verticies[j] - 1].z);

				calculatedTangentData.push_back(tangent.x);
				calculatedTangentData.push_back(tangent.y);
				calculatedTangentData.push_back(tangent.z);

				calculatedBiTangentData.push_back(biTangent.x);
				calculatedBiTangentData.push_back(biTangent.y);
				calculatedBiTangentData.push_back(biTangent.z);
			}
		}

		NumberOfFaces = faceData.size();
		NumberOfVerticies = NumberOfFaces * 3;

		vertices = unPackedVertexData;
		normals = unPackedNormalData;

		//SEND DATA TO OPENGL
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO_Verticies);
		glGenBuffers(1, &VBO_UVs);
		glGenBuffers(1, &VBO_Normals);
		glGenBuffers(1, &VBO_Tangents);
		glGenBuffers(1, &VBO_BiTangents);
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(0); //Vertex
		glEnableVertexAttribArray(1); //UVs
		glEnableVertexAttribArray(2); //Normals
		glEnableVertexAttribArray(3); //Tangents
		glEnableVertexAttribArray(4); //BiTangents

		glBindBuffer(GL_ARRAY_BUFFER, VBO_Verticies);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedVertexData.size(), &unPackedVertexData[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedTextureData.size(), &unPackedTextureData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedNormalData.size(), &unPackedNormalData[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Tangents);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* calculatedTangentData.size(), &calculatedTangentData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, VBO_BiTangents);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* calculatedBiTangentData.size(), &calculatedBiTangentData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)4, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
		
		//PUSH TO LIST

		Mesh* tempMesh = new Mesh();
		
		tempMesh->VBO_Normals = VBO_Normals;
		tempMesh->VBO_UVs = VBO_UVs;
		tempMesh->VBO_Verticies = VBO_Verticies;
		tempMesh->VBO_Tangents = VBO_Tangents;
		tempMesh->VBO_BiTangents = VBO_BiTangents;
		tempMesh->VAO = VAO;
			
		tempMesh->NumberOfFaces = NumberOfFaces;
		tempMesh->NumberOfVerticies = NumberOfVerticies;

		tempMesh->vertices = vertices;
		tempMesh->normals = normals;

		listOfMeshes[name] = tempMesh;

		//---------------------------------------------

		//CLEANUP
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		normalData.clear();
		textureData.clear();
		vertexData.clear();
		unPackedNormalData.clear();
		unPackedTextureData.clear();
		unPackedVertexData.clear();
		calculatedTangentData.clear();
		calculatedBiTangentData.clear();

		return true;
	}

	//UNLOADS ALL MESHES
	void Mesh::Unload()
{
		for (auto itr = listOfMeshes.begin(), itrEnd = listOfMeshes.end(); itr != itrEnd; itr++)
		{
			
			glDeleteBuffers(1, &(*itr).second->VBO_Normals);
			glDeleteBuffers(1, &(*itr).second->VBO_UVs);
			glDeleteBuffers(1, &(*itr).second->VBO_Verticies);
			glDeleteVertexArrays(1, &(*itr).second->VAO);

			(*itr).second->VBO_Normals = 0;
			(*itr).second->VBO_UVs = 0;
			(*itr).second->VBO_Verticies = 0;
			(*itr).second->VAO = 0;
	
			(*itr).second->NumberOfFaces = 0;
			(*itr).second->NumberOfVerticies = 0;
		}
}

	//ACCESSOR FUNCTIONS
	unsigned int Mesh::getNumberOfVerticies()
	{
		return NumberOfVerticies;
	}

	Mesh* Mesh::getMeshPtr()
	{
		if (meshPtr == nullptr)
			meshPtr = new Mesh();
		return meshPtr;
	}
}
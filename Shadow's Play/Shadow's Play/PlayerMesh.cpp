#include "PlayerMesh.h"
#include "glm\common.hpp"
#include "glm\matrix.hpp"

#define CHAR_BUFFER_SIZE 128
#define BUFFER_OFFSET(i) ((char *)0 + (i))

namespace ENG
{
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
	bool PlayerMesh::LoadFromFile(const std::string name, const std::string &fileA, const std::string &fileB)
	{

		std::ifstream inputA;
		std::ifstream inputB;

		inputA.open(fileA);

		if (!inputA)
		{
			std::cout << "Could not open the first file." << std::endl;
			return false;
		}

		char inputString[CHAR_BUFFER_SIZE];

		//PACKED DATA
		std::vector<glm::vec3> vertexData;
		std::vector<glm::vec2> textureData;
		std::vector<glm::vec3> normalData;

		std::vector<MeshFace> faceData;

		//UNPACKED DATA
		std::vector<float> unPackedVertexDataA;
		std::vector<float> unPackedTextureData;
		std::vector<float> unPackedNormalDataA;
		std::vector<float> calculatedTangentData;
		std::vector<float> calculatedBiTangentData;
		std::vector<float> unPackedVertexDataB;
		std::vector<float> unPackedNormalDataB;

		//READ/RECORD THE DATA
		while (!inputA.eof())
		{
			inputA.getline(inputString, CHAR_BUFFER_SIZE);

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
			else if (strstr(inputString, "f") != nullptr)
			{
				MeshFace temp;

				std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
					&temp.verticies[0], &temp.textureUVs[0], &temp.normals[0],
					&temp.verticies[1], &temp.textureUVs[1], &temp.normals[1],
					&temp.verticies[2], &temp.textureUVs[2], &temp.normals[2]);

				faceData.push_back(temp);
			}
		}

		inputA.close();

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

			float f = 1.0f / (deltaUVA.x * deltaUVB.y - deltaUVA.y * deltaUVB.x);
			glm::vec3 tangent, biTangent;
			tangent = f * (deltaPosA * deltaUVB.y - deltaPosB * deltaUVA.y);
			biTangent = f * (deltaPosB * deltaUVA.x - deltaPosA * deltaUVB.x);

			for (unsigned j = 0; j < 3; j++)
			{
				unPackedNormalDataA.push_back(normalData[faceData[i].normals[j] - 1].x);
				unPackedNormalDataA.push_back(normalData[faceData[i].normals[j] - 1].y);
				unPackedNormalDataA.push_back(normalData[faceData[i].normals[j] - 1].z);

				unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
				unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);

				unPackedVertexDataA.push_back(vertexData[faceData[i].verticies[j] - 1].x);
				unPackedVertexDataA.push_back(vertexData[faceData[i].verticies[j] - 1].y);
				unPackedVertexDataA.push_back(vertexData[faceData[i].verticies[j] - 1].z);

				calculatedTangentData.push_back(tangent.x);
				calculatedTangentData.push_back(tangent.y);
				calculatedTangentData.push_back(tangent.z);

				calculatedBiTangentData.push_back(biTangent.x);
				calculatedBiTangentData.push_back(biTangent.y);
				calculatedBiTangentData.push_back(biTangent.z);
			}
		}

		verticesA = unPackedVertexDataA;
		normalsA = unPackedNormalDataA;

		vertexData.clear();
		textureData.clear();
		normalData.clear();

		faceData.clear();

		//Reading from the second file now...
		inputB.open(fileB);

		if (!inputB)
		{
			std::cout << "Could not open the second file." << std::endl;
			return false;
		}

		//READ/RECORD THE DATA
		while (!inputB.eof())
		{
			inputB.getline(inputString, CHAR_BUFFER_SIZE);

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
			else if (strstr(inputString, "f") != nullptr)
			{
				MeshFace temp;

				std::sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
					&temp.verticies[0], &temp.textureUVs[0], &temp.normals[0],
					&temp.verticies[1], &temp.textureUVs[1], &temp.normals[1],
					&temp.verticies[2], &temp.textureUVs[2], &temp.normals[2]);

				faceData.push_back(temp);
			}
		}

		inputB.close();

		//UNPACK THE DATA
		for (unsigned i = 0; i < faceData.size(); i++)
		{
			glm::vec3 vertexA, vertexB, vertexC;
			//glm::vec2 uvA, uvB, uvC;

			vertexA = vertexData[faceData[i].verticies[0] - 1];
			vertexB = vertexData[faceData[i].verticies[1] - 1];
			vertexC = vertexData[faceData[i].verticies[2] - 1];

			//uvA = textureData[faceData[i].textureUVs[0] - 1];
			//uvB = textureData[faceData[i].textureUVs[1] - 1];
			//uvC = textureData[faceData[i].textureUVs[2] - 1];

			//glm::vec3 deltaPosA, deltaPosB;
			//glm::vec2 deltaUVA, deltaUVB;

			//deltaPosA = vertexB - vertexA;
			//deltaPosB = vertexC - vertexA;

			//deltaUVA = uvB - uvA;
			//deltaUVB = uvC - uvA;

			//float f = 1.0f / (deltaUVA.x * deltaUVB.y - deltaUVA.y * deltaUVB.x);
			//glm::vec3 tangent, biTangent;
			//tangent = f * (deltaPosA * deltaUVB.y - deltaPosB * deltaUVA.y);
			//biTangent = f * (deltaPosB * deltaUVA.x - deltaPosA * deltaUVB.x);

			for (unsigned j = 0; j < 3; j++)
			{
				unPackedNormalDataB.push_back(normalData[faceData[i].normals[j] - 1].x);
				unPackedNormalDataB.push_back(normalData[faceData[i].normals[j] - 1].y);
				unPackedNormalDataB.push_back(normalData[faceData[i].normals[j] - 1].z);

				//unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
				//unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);

				unPackedVertexDataB.push_back(vertexData[faceData[i].verticies[j] - 1].x);
				unPackedVertexDataB.push_back(vertexData[faceData[i].verticies[j] - 1].y);
				unPackedVertexDataB.push_back(vertexData[faceData[i].verticies[j] - 1].z);

				//calculatedTangentData.push_back(tangent.x);
				//calculatedTangentData.push_back(tangent.y);
				//calculatedTangentData.push_back(tangent.z);

				//calculatedBiTangentData.push_back(biTangent.x);
				//calculatedBiTangentData.push_back(biTangent.y);
				//calculatedBiTangentData.push_back(biTangent.z);
			}
		}

		verticesB = unPackedVertexDataB;
		normalsB = unPackedNormalDataB;

		NumberOfFaces = faceData.size();
		NumberOfVerticies = NumberOfFaces * 3;

		//SEND DATA TO OPENGL
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO_VerticiesA);
		glGenBuffers(1, &VBO_NormalsA);
		glGenBuffers(1, &VBO_UVs);
		glGenBuffers(1, &VBO_Tangents);
		glGenBuffers(1, &VBO_BiTangents);
		glGenBuffers(1, &VBO_VerticiesB);
		glGenBuffers(1, &VBO_NormalsB);
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(0); //Vertex
		glEnableVertexAttribArray(1); //UVs
		glEnableVertexAttribArray(2); //Normals
		glEnableVertexAttribArray(3); //Tangents
		glEnableVertexAttribArray(4); //BiTangents
		glEnableVertexAttribArray(5); //VertexB
		glEnableVertexAttribArray(6); //NormalB

		glBindBuffer(GL_ARRAY_BUFFER, VBO_VerticiesA); //loc 0
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedVertexDataA.size(), &unPackedVertexDataA[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO_NormalsA); //loc 2
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedNormalDataA.size(), &unPackedNormalDataA[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedTextureData.size(), &unPackedTextureData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, VBO_Tangents);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* calculatedTangentData.size(), &calculatedTangentData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, VBO_BiTangents);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* calculatedBiTangentData.size(), &calculatedBiTangentData[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)4, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, VBO_VerticiesB); //loc 5
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedVertexDataB.size(), &unPackedVertexDataB[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)5, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

		glBindBuffer(GL_ARRAY_BUFFER, VBO_NormalsB); //loc 6
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* unPackedNormalDataB.size(), &unPackedNormalDataB[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer((GLuint)6, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

		//---------------------------------------------

		//CLEANUP
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		normalData.clear();
		textureData.clear();
		vertexData.clear();
		unPackedNormalDataA.clear();
		unPackedTextureData.clear();
		unPackedVertexDataA.clear();
		calculatedTangentData.clear();
		calculatedBiTangentData.clear();
		unPackedVertexDataB.clear();
		unPackedNormalDataB.clear();

		return true;
	}

	//ACCESSOR FUNCTIONS
	unsigned int PlayerMesh::getNumberOfVerticies()
	{
		return NumberOfVerticies;
	}
}
//Cameron van Velzen - 100591663
#pragma once

#include "GL\glew.h"
#include <string>
#include <unordered_map>
#include "SFML\OpenGL.hpp"
#include "Lights.h"

namespace ENG
{
	//SHADER LOADER CLASS
	class Shader
	{
	public:
		Shader();
		~Shader();
		
		bool load(const std::string, const std::string &, const std::string &);
		bool isLoaded() const;
		
		void unload();
		bool linkProgram();

		void bind();
		void unBind();

		void addAttribute(unsigned int, const std::string &);

		int getAttributeLocation(const std::string &);

		int getUniformLocation(const std::string &);

		void sendUniform(const std::string &, int);
		void sendUniform(const std::string &, unsigned int);
		void sendUniform(const std::string &, float);
		void sendUniform(const std::string &, const glm::vec2 &);
		void sendUniform(const std::string &, const glm::vec3 &);
		void sendUniform(const std::string &, const glm::vec4 &);
		void sendUniformMat3(const std::string &, float*, bool);
		void sendUniformMat4(const std::string &, float*, bool);
		void sendUniformPointLight(const std::string &, PointLight *);
		void sendArrayOfPointLights(const std::string &, PointLight *, int);

	private:
		bool isInit = false;
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint program;

		std::string readFile(const std::string &);
		bool compileShader(GLuint) const;
		void outputShaderLog(GLuint) const;
		void outputProgramLog() const;
	};
}
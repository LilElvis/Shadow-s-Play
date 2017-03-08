#include "Shader.h"
#include <fstream>
#include <iostream>

namespace ENG
{
	Shader::Shader()
	{

	}

	Shader::~Shader()
	{
		if (isInit)
		{
			unload();
		}
	}

	//LOADS A SHADER PROGRAM FROM A VERTEX AND FRAGMENT SHADER 
	bool Shader::load(const std::string name, const std::string & vertexFile, const std::string & fragmentFile)
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		program = glCreateProgram();

		std::string source = readFile(vertexFile);
		const GLchar *temp = static_cast<const GLchar *>(source.c_str());
		glShaderSource(vertexShader, 1, &temp, NULL);
		
		source = readFile(fragmentFile);
		temp = static_cast<const GLchar *>(source.c_str());
		glShaderSource(fragmentShader, 1, &temp, NULL);

		if (!compileShader(vertexShader))
		{
			std::cout << "Vertex Shader failed to compile.\n";

			outputShaderLog(vertexShader);
			unload();

			return false;
		}

		if (!compileShader(fragmentShader))
		{
			std::cout << "Fragment Shader failed to compile.\n";

			outputShaderLog(fragmentShader);
			unload();

			return false;
		}

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		if (!linkProgram())
		{
			std::cout << "Shader program failed to link.\n";

			outputProgramLog();
			unload();

			return false;
		}

		isInit = true;

		return true;
	}

	 bool Shader::isLoaded() const
	{
		 return isInit;
	}

	 //UNLOADS/UNLINKS SHADER PROGRAMS 
	void Shader::unload()
	{
		if (vertexShader != 0)
		{
			glDetachShader(program, vertexShader);
			glDeleteShader(vertexShader);
			vertexShader = 0;
		}

		if (fragmentShader != 0)
		{
			glDetachShader(program, fragmentShader);
			glDeleteShader(fragmentShader);
			fragmentShader = 0;
		}

		if (program != 0)
		{
			glDeleteProgram(program);
			program = 0;
		}

		isInit = false;
	}

	//LINKS A SHADER PROGRAM
	bool Shader::linkProgram()
	{
		glLinkProgram(program);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		return success == GL_TRUE;
	}

	//BINDING & UNBINDING
	void Shader::bind()
	{
		glUseProgram(program);
	}

	void Shader::unBind()
	{
		glUseProgram(0);
	}

	//UTILITY FUNCTIONS
	void Shader::addAttribute(unsigned int index, const std::string & attribName)
	{
		glBindAttribLocation(program, index, attribName.c_str());
	}

	int Shader::getAttributeLocation(const std::string & attribName)
	{
		return glGetAttribLocation(program, attribName.c_str());
	}

	int Shader::getUniformLocation(const std::string & uniformName)
	{
		return glGetUniformLocation(program, uniformName.c_str());
	}

	void Shader::sendUniform(const std::string & name, int integer)
	{
		GLint location = getUniformLocation(name);
		glUniform1i(location, integer);
	}

	void Shader::sendUniform(const std::string & name, unsigned int unsigned_integer)
	{
		GLint location = getUniformLocation(name);
		glUniform1ui(location, unsigned_integer);
	}

	void Shader::sendUniform(const std::string & name, float scalar)
	{
		GLint location = getUniformLocation(name);
		glUniform1f(location, scalar);
	}

	void Shader::sendUniform(const std::string & name, const glm::vec2 &vector)
	{
		GLint location = getUniformLocation(name);
		glUniform2f(location, vector.x, vector.y);
	}

	void Shader::sendUniform(const std::string & name, const glm::vec3 &vector)
	{
		GLint location = getUniformLocation(name);
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void Shader::sendUniform(const std::string &name, const glm::vec4 &vector)
	{
		GLint location = getUniformLocation(name);
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void Shader::sendUniformMat3(const std::string & name, float* matrix, bool transpose)
	{
		GLint location = getUniformLocation(name);
		glUniformMatrix3fv(location, 1, transpose, matrix);
	}

	void Shader::sendUniformMat4(const std::string & name, float* matrix, bool transpose)
	{
		GLint location = getUniformLocation(name);
		glUniformMatrix4fv(location, 1, transpose, matrix);
	}

	void Shader::sendUniformPointLight(const std::string & name, PointLight* light, int index)
	{
		sendUniform(name + "[" + std::to_string(index) + "].position", light->position);
		sendUniform(name + "[" + std::to_string(index) + "].color", light->color);
		sendUniform(name + "[" + std::to_string(index) + "].aConstant", light->aConstant);
		sendUniform(name + "[" + std::to_string(index) + "].aLinear", light->aLinear);
		sendUniform(name + "[" + std::to_string(index) + "].aQuadratic", light->aQuadratic);
	}
	
	void Shader::sendUniformProjectedTexLight(const std::string & name, ProjectedTexLight* light, int texSlot, int index)
	{
		sendUniformMat4(name + "[" + std::to_string(index) + "].lightViewMatrix", &light->lightViewMatrix[0][0], false);
		sendUniform(name + "[" + std::to_string(index) + "].lightTexture", texSlot);
		sendUniform(name + "[" + std::to_string(index) + "].lightDirection", light->lightDirection);
		sendUniform(name + "[" + std::to_string(index) + "].intensity", light->intensity);
	}

	//READS SHADER FILES
	std::string Shader::readFile(const std::string &fileName)
	{
		std::ifstream inStream(fileName);

		if (!inStream.good())
		{
			std::cout << "Shader file not found.\n";
			return std::string();
		}

		std::string data(std::istreambuf_iterator<char>(inStream), (std::istreambuf_iterator<char>()));
		return data;
	}

	//COMPILE GLSL SHADERS
	bool Shader::compileShader(GLuint shader) const
	{
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		return success == GL_TRUE;
	}

	//OUTPUT GLSL INFO TO CONSOLE
	void Shader::outputShaderLog(GLuint shader) const
	{
		std::vector<char> infoLog;
		infoLog.resize(512);


		GLint infoLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		glGetShaderInfoLog(shader, sizeof(char) * 512, &infoLen, &infoLog[0]);

		std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
	}

	void Shader::outputProgramLog() const
	{
		std::vector<char> infoLog;
		infoLog.resize(512);


		GLint infoLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

		glGetProgramInfoLog(program, sizeof(char) * 512, &infoLen, &infoLog[0]);

		std::cout << std::string(infoLog.begin(), infoLog.end()) << std::endl;
	}
}
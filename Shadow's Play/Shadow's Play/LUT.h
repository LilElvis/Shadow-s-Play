#pragma once

#include "GL\glew.h"
#include <string>
#include <unordered_map>
#include "SFML\OpenGL.hpp"

namespace ENG
{
	class LUT
	{
	public:
		LUT();
		~LUT();

		void bind(GLenum textureUnit = GL_TEXTURE0);

		void loadData(std::string filename);

	private:
		GLuint handle;
		int dataSize;

	};
}
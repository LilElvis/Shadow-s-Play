#pragma once
#include "Window.h"

namespace ENG
{
	class FrameBufferObject
	{
	public:
		FrameBufferObject();
		~FrameBufferObject();

		void Init(unsigned int, unsigned int, unsigned int = 1);

		void initDepthTexture(GLint = GL_NEAREST, GLint = GL_CLAMP_TO_EDGE);
		void initColorTexure(unsigned int, GLint = GL_NEAREST, GLint = GL_CLAMP_TO_EDGE);

		void UnloadFBO();
		void ClearFBO();

		void Bind();
		void BindDepthAsTexture(GLenum);
		void BindColorAsTexture(GLenum, unsigned int);
		void Unbind();

		void DrawToBackBuffer();

	private:
		GLuint FBO;
		GLuint DepthAttachment;
		GLuint * ColorAttachments;
		GLenum * Buffers;
		unsigned int numColorAttachments;
		unsigned int Width;
		unsigned int Height;
	};
}
//Cameron van Velzen - 100591663
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
		void initColorTexture(unsigned int, GLenum = GL_RGBA8, GLint = GL_NEAREST, GLint = GL_CLAMP_TO_EDGE);

		void UnloadFBO();
		void ClearFBO();

		void Bind();
		void BindDepthAsTexture(GLenum);
		void BindColorAsTexture(GLenum, unsigned int);
		void Unbind();

		GLuint getColorAttachment(unsigned int);

		void DrawToBackBuffer();

		void BlitColorToTargetBuffer(FrameBufferObject*);
		void BlitDepthToTargetBuffer(FrameBufferObject*);

		GLuint getLayerNumber();

		void Check();

	private:
		GLuint FBO;
		GLuint DepthAttachment;
		GLuint* ColorAttachments;
		GLenum* Buffers;
		unsigned int numColorAttachments;
		unsigned int Width;
		unsigned int Height;
	};
}
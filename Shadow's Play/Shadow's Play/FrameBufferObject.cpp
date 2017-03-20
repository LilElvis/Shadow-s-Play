#include "FrameBufferObject.h"

#define GL_DONT_GIVE_A_FUCK GL_DONT_CARE

namespace ENG
{
	FrameBufferObject::FrameBufferObject()
	{

	}

	FrameBufferObject::~FrameBufferObject()
	{
		UnloadFBO();
	}

	void FrameBufferObject::Init(unsigned int width, unsigned int height, unsigned int numRenderTargets)
	{
		numColorAttachments = numRenderTargets;
		Width = width;
		Height = height;

		glGenFramebuffers(1, &FBO);
		ColorAttachments = new GLuint[numColorAttachments];
		Buffers = new GLenum[numColorAttachments];

		for (int i = 0; i < numColorAttachments; ++i)
		{
			Buffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
	}

	void FrameBufferObject::initDepthTexture(GLint filterMode, GLint wrapMode)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glGenTextures(1, &DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, Width, Height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);
		Unbind();
	}

	void FrameBufferObject::initColorTexture(unsigned int index, GLenum dataType, GLint filterMode, GLint wrapMode)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glGenTextures(1, &ColorAttachments[index]);
		glBindTexture(GL_TEXTURE_2D, ColorAttachments[index]);
		glTexStorage2D(GL_TEXTURE_2D, 1, dataType, Width, Height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

		glFramebufferTexture2D(GL_FRAMEBUFFER, Buffers[index], GL_TEXTURE_2D, ColorAttachments[index], 0);
		Unbind();
	}

	void FrameBufferObject::UnloadFBO()
	{
		if (Buffers != nullptr)
		{
			delete[] Buffers;
			Buffers = nullptr;
		}
		if (ColorAttachments != nullptr)
		{
			for (int i = 0; i < numColorAttachments; ++i)
				glDeleteTextures(1, &ColorAttachments[i]);
			delete[] ColorAttachments;
			ColorAttachments = nullptr;
		}
		if (DepthAttachment != GL_NONE)
		{
			glDeleteTextures(1, &DepthAttachment);
			DepthAttachment = GL_NONE;
		}
		numColorAttachments = GL_NONE;
	}

	void FrameBufferObject::ClearFBO()
	{
		GLbitfield temp = GL_NONE;
		if (DepthAttachment != GL_NONE)
			temp = temp | GL_DEPTH_BUFFER_BIT;

		if (ColorAttachments != nullptr)
			temp = temp | GL_COLOR_BUFFER_BIT;

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClear(temp);
		Unbind();
	}

	void FrameBufferObject::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glDrawBuffers(numColorAttachments, Buffers);
	}

	void FrameBufferObject::BindDepthAsTexture(GLenum activeTexture)
	{
		glActiveTexture(activeTexture);
		glBindTexture(GL_TEXTURE_2D, DepthAttachment);
	}

	void FrameBufferObject::BindColorAsTexture(GLenum activeTexture, unsigned int index)
	{
		glActiveTexture(activeTexture);
		glBindTexture(GL_TEXTURE_2D, ColorAttachments[index]);
	}

	void FrameBufferObject::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	GLuint FrameBufferObject::getColorAttachment(unsigned int index)
	{
		return ColorAttachments[index];
	}

	void FrameBufferObject::DrawToBackBuffer()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		Unbind();
	}

	void FrameBufferObject::BlitColorToTargetBuffer(FrameBufferObject* anFBO)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, anFBO->getLayerNumber());

		glBlitFramebuffer(0, 0, Width, Height, 0, 0, anFBO->Width, anFBO->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		Unbind();
	}

	void FrameBufferObject::BlitDepthToTargetBuffer(FrameBufferObject* anFBO)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, anFBO->getLayerNumber());

		glBlitFramebuffer(0, 0, Width, Height, 0, 0, anFBO->Width, anFBO->Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		Unbind();
	}

	GLuint FrameBufferObject::getLayerNumber()
	{
		return FBO;
	}

	unsigned int FrameBufferObject::getNumberOfColorAttatchments()
	{
		return numColorAttachments;
	}

	void FrameBufferObject::Check()
	{

	}
}

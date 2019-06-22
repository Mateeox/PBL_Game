#include "PostProcessing/FrameBuffer.hpp"
#include <iostream>

FrameBuffer::FrameBuffer(unsigned Width, unsigned Height)
{
    glGenFramebuffers(1, &FramebufferObject);
    BindFrameBuffer();

    glGenTextures(1, &frameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);

    glGenRenderbuffers(1, &RenderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferObject);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);                                  // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBufferObject); // now actually attach it

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferObject);
}

void FrameBuffer::bindBack()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	
}
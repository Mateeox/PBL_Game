#pragma once

#include <GL/gl3w.h>

class FrameBuffer
{

    unsigned FramebufferObject;
    unsigned frameBufferTexture;
    unsigned RenderBufferObject;

public:
    FrameBuffer(unsigned Width, unsigned Height);
    void BindFrameBuffer();
    void bindBack();
    void bindTexture();
};
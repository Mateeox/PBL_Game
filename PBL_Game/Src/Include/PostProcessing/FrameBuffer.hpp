#pragma once

#include <GL/gl3w.h>

class FrameBuffer
{

    unsigned FramebufferObject;
    unsigned RenderBufferObject;

public:
    unsigned frameBufferTexture;
    FrameBuffer(unsigned Width, unsigned Height);
    void BindFrameBuffer();
    void bindBack();
    void bindTexture();
};
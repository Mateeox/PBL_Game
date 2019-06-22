#include "PostProcessing/FrameBuffer.hpp"

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &FramebufferObject);
}


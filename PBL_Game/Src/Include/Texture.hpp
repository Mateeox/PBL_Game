#pragma once
#include <GL/gl3w.h>
#include <string>

class Texture
{

  public:
    unsigned  texture;
    unsigned mipmap_param;
    std::string path;
    void Bind();
    bool Load();
    void Bind(unsigned TextureUnit);
    Texture(const char *PATH, unsigned mipmap_param);
    ~Texture();
};

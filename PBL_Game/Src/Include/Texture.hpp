#pragma once
#include <GL/gl3w.h>
#include <string>
#include <assimp/scene.h>

class Texture
{

  public:
    unsigned  texture;
    unsigned mipmap_param;
    aiTexture** textures;
    std::string path;
    void Bind();
    bool Load();
    void Bind(unsigned TextureUnit);
    Texture(const char *PATH, unsigned mipmap_param);
    Texture(const char *PATH, unsigned mipmap_param, aiTexture** textures);
    ~Texture();
};
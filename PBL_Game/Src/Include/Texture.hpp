
#include <GL/gl3w.h>

class Texture
{

  public:
    unsigned int texture;
    void Bind();
    Texture(const char *PATH, GLenum XD);
    ~Texture();
};

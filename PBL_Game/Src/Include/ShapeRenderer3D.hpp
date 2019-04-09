#pragma once
#include "Drawable.hpp"
#include "Texture.hpp"

class ShapeRenderer3D :public Drawable 
{

    float* g_vertex_buffer_data;
    unsigned int * indices;
    int  g_vertex_buffer_data_size;
    int  indices_size;

    int what_Draw_use;
    void Draw_Elements();
    void Draw_Arrays();
    Texture* texture;

    public:

    ShapeRenderer3D(float* g_vertex_buffer_data,unsigned int * aindices,int array_size,int aindices_size,Shader& ShaderProgram,Texture* Texture);
    ShapeRenderer3D(float * g_vertex_buffer_date,int indices_size,Shader & shaderProgram,Texture * texture);
    void Draw(glm::mat4 &  transform);
   

    ComponentSystem::ComponentType GetComponentType() override;


    
    ~ShapeRenderer3D();


};


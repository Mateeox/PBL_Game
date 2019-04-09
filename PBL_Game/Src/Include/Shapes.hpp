#pragma once

namespace Shapes
{


struct GShape
{
	float* Verticies;
	unsigned int* indices; 

};


	  static float RainBow_Triangle[] = { 
		//position          //color          //TexCoords
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
		 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f
	};

	static unsigned int RB_Triangle_indices[] = {
        0, 1, 2, // first triangle
    };



		static float RainBow_Square[] = { 
		//position          //color             //TexCoords  
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

		static unsigned int RB_Square_indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };






	static  float RainBow_Cube[] = {
    // front
    -1.0, -1.0,  1.0,  1.0f, 0.0f, 0.0f,  0.0, 0.0,
     1.0, -1.0,  1.0,  0.0f, 1.0f, 0.0f,  1.0, 0.0,
     1.0,  1.0,  1.0,  0.0f, 0.0f, 1.0f,  1.0, 1.0,
    -1.0,  1.0,  1.0,  1.0f, 1.0f, 0.0f,  0.0, 1.0,
    // top
    -1.0,  1.0,  1.0, 1.0f, 0.0f, 0.0f,   0.0, 0.0,
     1.0,  1.0,  1.0, 0.0f, 1.0f, 0.0f,   1.0, 0.0,
     1.0,  1.0, -1.0, 0.0f, 0.0f, 1.0f,   1.0, 1.0,
    -1.0,  1.0, -1.0, 1.0f, 1.0f, 0.0f,   0.0, 1.0,
    // back
     1.0, -1.0, -1.0, 1.0f, 0.0f, 0.0f,    0.0, 0.0,
    -1.0, -1.0, -1.0, 0.0f, 1.0f, 0.0f,    1.0, 0.0,
    -1.0,  1.0, -1.0, 0.0f, 0.0f, 1.0f,    1.0, 1.0,
     1.0,  1.0, -1.0, 1.0f, 1.0f, 0.0f,    0.0, 1.0,
    // bottom
    -1.0, -1.0, -1.0, 1.0f, 0.0f, 0.0f,    0.0, 0.0,
     1.0, -1.0, -1.0, 0.0f, 1.0f, 0.0f,    1.0, 0.0,
     1.0, -1.0,  1.0, 0.0f, 0.0f, 1.0f,    1.0, 1.0,
    -1.0, -1.0,  1.0, 1.0f, 1.0f, 0.0f,    0.0, 1.0,
    // left
    -1.0, -1.0, -1.0, 1.0f, 0.0f, 0.0f,    0.0, 0.0,  
    -1.0, -1.0,  1.0, 0.0f, 1.0f, 0.0f,    1.0, 0.0,
    -1.0,  1.0,  1.0, 0.0f, 0.0f, 1.0f,    1.0, 1.0,
    -1.0,  1.0, -1.0, 1.0f, 1.0f, 0.0f,    0.0, 1.0,
    // right
     1.0, -1.0,  1.0, 1.0f, 0.0f, 0.0f,   0.0, 0.0,
     1.0, -1.0, -1.0, 0.0f, 1.0f, 0.0f,   1.0, 0.0,
     1.0,  1.0, -1.0, 0.0f, 0.0f, 1.0f,   1.0, 1.0,
     1.0,  1.0,  1.0, 1.0f, 1.0f, 0.0f,   0.0, 1.0
  
};

static unsigned int RB_Cube_indices[] = {	// front
	  // front
     0,  1,  2,
     2,  3,  0,
    // top
     4,  5,  6,
     6,  7,  4,
    // back
     8,  9, 10,
    10, 11,  8,
    // bottom
    12, 13, 14,
    14, 15, 12,
    // left
    16, 17, 18,
    18, 19, 16,
    // right
    20, 21, 22,
    22, 23, 20,
        };

	static GShape RB_Triangle = {RainBow_Triangle,RB_Triangle_indices};
	static GShape RB_Square = {RainBow_Square,RB_Square_indices};
    static GShape RB_Cube = {RainBow_Cube,RB_Cube_indices};


}
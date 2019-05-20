# CMake entry point
cmake_minimum_required (VERSION 3.9.3)
project (PBL_Game)

find_package(OpenGL REQUIRED)
 
if(UNIX)
  set(CMAKE_C_COMPILER /usr/bin/gcc-8 CACHE PATH "" FORCE)
  set(CMAKE_CXX_COMPILER /usr/bin/g++-8 CACHE PATH "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)

# Compile external dependencies 
add_subdirectory (external)

include_directories(
	Src/Include/
	external/Imgui/Include/
	external/Imgui/examples/libs/gl3w/
	external/glfw-3.2.1/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-4.1.0/include/
	external/tinyxml2/
	Build/external/assimp-4.1.0/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	assimp
	tinyxml2
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(PBL_Game 
    PBL_Game.cpp
    Src/Include/Transform.cpp
    Src/Include/Game.cpp
    Src/Include/GameObject.cpp 
	  Src/Include/Transform.cpp
	  Src/Include/Window.cpp
	  Src/Include/Camera.cpp
	  Src/Include/Shader.cpp
	  Src/Include/Texture.cpp
	  Src/Include/SceneNode.cpp
		Src/Include/Collider.cpp
		Src/Include/ConfigUtils.cpp
		#PathFinding
		Src/Include/PathFinding/PathFindingUtils.cpp
		Src/Include/PathFinding/MapTile.cpp
		Src/Include/PathFinding/MapTileRenderUtils.cpp
	  #Component
	  Src/Include/Component/ShapeRenderer3D.cpp
	  Src/Include/Component/Mesh.cpp
	  Src/Include/Component/Model.cpp
	  Src/Include/Component/AnimatedModel.cpp
	  Src/Include/Component/BoneUtils.cpp
	  #Stb_image
	  Src/Include/stb_image.cpp
	  #ImGui
	  external/Imgui/examples/libs/gl3w/GL/gl3w.c
	  external/Imgui/Include/imgui.cpp
	  external/Imgui/Include/imgui_impl_glfw.cpp
	  external/Imgui/Include/imgui_impl_opengl3.cpp
	  external/Imgui/Include/imgui_draw.cpp
	  external/Imgui/Include/imgui_widgets.cpp
	
)

target_link_libraries(PBL_Game
	${ALL_LIBS}
)

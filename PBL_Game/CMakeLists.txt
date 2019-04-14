# CMake entry point
cmake_minimum_required (VERSION 3.9.3)
project (Tutorials)

find_package(OpenGL REQUIRED)
 
set(CMAKE_CXX_STANDARD 14)
 
# Compile external dependencies 
add_subdirectory (external)

include_directories(
	Src/Include/
	external/Imgui/Include/
	external/Imgui/examples/libs/gl3w/
	external/glfw-3.2.1/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	.
)




set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	assimp
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
	#Component
	Src/Include/Component/ShapeRenderer3D.cpp
	Src/Include/Component/Mesh.cpp
	Src/Include/Component/Model.cpp
	#Stb_image
	Src/Include/stb_image.cpp
	#Model
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

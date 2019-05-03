#include "Texture.hpp"
#include <iostream>

#include "stb_image.hpp"

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::Bind(unsigned TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

bool Texture::Load()
{
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);						  // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
																  // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap_param);
	// load image, create texture and generate mipmaps
	int width, height, components_per_pixel;
	unsigned char *data = nullptr;
	aiTexture *embededData = nullptr;
	unsigned format = 0;

	if (textures != nullptr)
	{

		embededData = textures[(int)(path.c_str()[1] - '0')];
		data = stbi_load_from_memory(reinterpret_cast<unsigned char *>(embededData->pcData), embededData->mWidth, &width, &height, &components_per_pixel, 0);
	}
	else
	{
		data = stbi_load(path.c_str(), &width, &height, &components_per_pixel, STBI_rgb_alpha);
	}

	if (components_per_pixel == 1)
		format = GL_RED;
	else if (components_per_pixel == 3)
		format = GL_RGB;
	else if (components_per_pixel == 4)
		format = GL_RGBA;

	if (data != nullptr)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 1;
	}

	if (data != nullptr)
	{
		stbi_image_free(data);
	}

	return 0;
}

Texture::Texture(const char *PATH, unsigned amipmap_param)
{
	textures = nullptr;
	path = PATH;
	mipmap_param = amipmap_param;
}

Texture::Texture(const char *PATH, unsigned amipmap_param, aiTexture **aTextures)
{
	textures = aTextures;
	path = PATH;
	mipmap_param = amipmap_param;
}

Texture::~Texture()
{
}

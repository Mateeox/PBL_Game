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
	int width, height, nrChannels, components_per_pixel;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.

	unsigned char *data = nullptr;
	unsigned char *embdata = nullptr;
	aiTexture *embededData = nullptr;

	if (textures != nullptr)
	{

		embededData = textures[(int)(path.c_str()[1] - '0')];
		embdata = stbi_load_from_memory(reinterpret_cast<unsigned char *>(embededData->pcData), embededData->mWidth, &width, &height, &components_per_pixel, 0);
	}
	else
	{
		data = stbi_load(path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
	}

	if (data != nullptr || embededData != nullptr)
	{
		if (embededData != nullptr)
		{
			
			if (components_per_pixel == 3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, embdata);
			}
			else if (components_per_pixel == 4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, embdata);
			}
		}
		else
		{

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

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
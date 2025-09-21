#include "config.h"

Texture::Texture(const char* texture_filepath, bool flip_vertically, bool has_alpha)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	SetParameters();

	if (flip_vertically)
	{
		stbi_set_flip_vertically_on_load(true);
	}
	else
	{
		stbi_set_flip_vertically_on_load(false);
	}

	unsigned char* data = stbi_load(texture_filepath, &width, &height, &color_channels, 0);
	
	if (data)
	{
		if (has_alpha)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture" << std::endl;
		return;
	}

	stbi_image_free(data);
};

Texture::Texture(const char* texture_filepath, const char* type, const unsigned int id)
{
	path = texture_filepath;
	this->type = type;
	ID = id;
}

void Texture::SetParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
};
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Texture
{
	public:
		// Texture ID
		std::string type;
		std::string path;
		unsigned int ID;
		int width, height, color_channels;

		Texture(const char* texture_filepath, bool flip_vertically, bool has_alpha);
		Texture(const char* texture_filepath, const char* type, const unsigned int id);

		void SetParameters();
};


#endif
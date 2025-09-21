#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include "mesh.h"

class VBO
{
	public:
		GLuint ID;
		VBO(Vertex* vertices, GLsizeiptr size);

		void Bind();
		void Unbind();
		void Delete();
};

class VAO
{
	public:
		GLuint ID;
		VAO();

		void LinkVBO(VBO VBO, GLuint layout);
		void Bind();
		void Unbind();
		void Delete();
};

class EBO
{
	public:
		GLuint ID;
		EBO(GLuint* indices, GLsizeiptr size);

		void Bind();
		void Unbind();
		void Delete();
};
#endif
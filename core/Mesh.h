#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
	public:
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(const std::vector<Vertex> vertices, const std::vector<GLuint> indices, const std::vector<Texture> textures,
			const Shader& shader = Shader("../OpenGL/shaders/default_vertex.vert", "../OpenGL/shaders/default_fragment.frag"));
		Mesh(const std::string& primitiveType, const Shader& shader = Shader("../OpenGL/shaders/default_vertex.vert", "../OpenGL/shaders/default_fragment.frag"));

		// Setters for transformations
		void SetPosition(const glm::vec3& position);
		void SetRotation(float angle, const glm::vec3& axis);
		void SetScale(const glm::vec3& scale);
		void SetShader(const Shader& shader);

		// Getters
		glm::vec3 GetPosition() const;
		Shader GetShader() const;
		std::vector<Vertex> GetVertices() const;

		glm::mat4 LocalToWorldmatrix() const;

		void Draw(const Camera& camera) const;

	private:

		std::string m_Type;
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::vec3 m_RotationAxis;
		float m_RotationAngle;

		Shader m_Shader;

		GLuint vaoID;
		GLuint vboID;
		GLuint eboID;

		unsigned int numVertices;
		std::vector<Vertex> m_vertices;
		std::vector<GLuint> m_indices;

		void Initialize();
};

#endif
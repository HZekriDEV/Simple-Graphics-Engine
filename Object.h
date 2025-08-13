#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gl_buffers.h"
#include "stb_image.h"
#include "shader.h"
#include "texture.h"
#include "Camera.h"
#include "Light.h"
#include "Color.h"
#include "mesh.h"
#include "model.h"
#include "Object.h"

struct Transform
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 eulerRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

class Object
{
	public:
		Object(const std::string name);
		unsigned int ID;
		std::string name;
		std::string meshType = "CUBE"; // Default mesh type
		std::vector<Object> childObjects;
		Transform transform;
		bool isPrimitive = true;


		void SetMesh(const Mesh& mesh);
		void SetMesh(const Model& model);
		void SetShader(const Shader& shader);
		Mesh GetMesh();
		void UpdatePosition(const glm::vec3& position);
		void UpdateRotation(const glm::vec3& angles);
		void UpdateScale(const glm::vec3& scale);
		void Render(const Camera& camera);

	private:
		Mesh m_mesh = Mesh("CUBE");
		Model m_model = Model("../OpenGL/assets/dragon.obj");
		Shader m_shader = Shader("../OpenGL/shaders/default_vertex.vert", "../OpenGL/shaders/default_fragment.frag");
};

#endif

#include "config.h"

Mesh::Mesh(const std::vector<Vertex> vertices, const std::vector<GLuint> indices, const std::vector<Texture> textures, const Shader& shader)
	: m_Shader(shader)
{
	m_Type = "CUSTOM";
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_RotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_RotationAngle = 0.0f;
	
	m_vertices = vertices;
	m_indices = indices;

	Initialize();
}

Mesh::Mesh(const std::string& primitiveType, const Shader& shader)
	: m_Shader(shader) 
{
	m_Type = primitiveType;
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_RotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	m_RotationAngle = 0.0f;

	Initialize();
}

void Mesh::SetPosition(const glm::vec3& position)
{
	m_Position = position;
}

void Mesh::SetRotation(float angle, const glm::vec3& axis)
{
	m_RotationAngle = angle;
	m_RotationAxis = axis;
}

void Mesh::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
}

void Mesh::SetShader(const Shader& shader)
{
	m_Shader = shader;
}

glm::vec3 Mesh::GetPosition() const
{
	return m_Position;
}

Shader Mesh::GetShader() const
{
	return m_Shader;
}

glm::mat4 Mesh::LocalToWorldmatrix() const
{
	// For the object (transforms from local to world coordinates)
	glm::mat4 model = glm::mat4(1.0f); // Start with identity

	model = glm::translate(model, m_Position);
	model = glm::rotate(model, glm::radians(m_RotationAngle), m_RotationAxis);
	model = glm::scale(model, m_Scale);
	return model;
}

void Mesh::Draw(const Camera& camera) const
{
	m_Shader.Activate();

	m_Shader.SetVec3("viewPos", camera.Position());

	// For the object (transforms from local to world coordinates)
	glm::mat4 model = LocalToWorldmatrix();

	// For transforming vertices in world coordinates to the cameras view coordinates
	glm::mat4 view = camera.ViewMatrix();

	// For projecting the view coordinates to the frustum, which gets turned to fragments
	glm::mat4 projection = projection = glm::perspective(glm::radians(camera.FOV()), (float)camera.screenWidth / (float)camera.screenHeight, 0.1f, 100.0f);

	m_Shader.SetMat4("model", model);
	m_Shader.SetMat4("view", view);
	m_Shader.SetMat4("projection", projection);

	/*unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		m_shader.SetInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}
	glActiveTexture(GL_TEXTURE0);*/

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Initialize()
{
	if (m_Type == "CUSTOM")
	{
		if (m_vertices.empty())
		{
			std::cout << "Error: CUSTOM Mesh has no vertices!" << std::endl;
			return;
		}

		numVertices = m_vertices.size();

		VAO VAO;
		VBO VBO(&m_vertices[0], m_vertices.size() * sizeof(Vertex));
		EBO EBO(&m_indices[0], m_indices.size() * sizeof(unsigned int));
		vaoID = VAO.ID;
		vboID = VBO.ID;
		eboID = EBO.ID;

		VAO.Bind();
		VBO.Bind();
		VAO.LinkVBO(VBO, 0);
		EBO.Bind();

		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
	}
	else if (m_Type == "CUBE")
	{
		Vertex vertices[] = {
			// Back face
			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)}, // Bottom-left
			{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f)}, // Bottom-right
			{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)}, // Top-right
			{glm::vec3(-0.5f, 0.5f, -0.5f),  glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f)}, // Top-left

			// Front face
			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f)}, // Bottom-left
			{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.0f)}, // Bottom-right
			{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f)}, // Top-right
			{glm::vec3(-0.5f, 0.5f,  0.5f),  glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 1.0f)}, // Top-left

			// Left face
			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)}, // Bottom-left
			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)}, // Bottom-right
			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)}, // Top-right
			{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)}, // Top-left

			// Right face
			{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)}, // Bottom-left
			{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)}, // Bottom-right
			{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)}, // Top-right
			{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)}, // Top-left

			// Bottom face
			{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)}, // Bottom-left
			{glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)}, // Bottom-right
			{glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f)}, // Top-right
			{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f)}, // Top-left

			// Top face
			{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f)}, // Bottom-left
			{glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)}, // Bottom-right
			{glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f)}, // Top-right
			{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f)}  // Top-left
		};

		GLuint indices[] = {
			// Back face
			0, 1, 2,
			2, 3, 0,

			// Front face
			4, 5, 6,
			6, 7, 4,

			// Left face
			8, 9, 10,
			10, 11, 8,

			// Right face
			12, 13, 14,
			14, 15, 12,

			// Bottom face
			16, 17, 18,
			18, 19, 16,

			// Top face
			20, 21, 22,
			22, 23, 20
		};

		numVertices = 36;

		VAO VAO;
		VBO VBO(&vertices[0], sizeof(vertices));
		EBO EBO(indices, sizeof(indices));
		vaoID = VAO.ID;
		vboID = VBO.ID;
		eboID = EBO.ID;

		VAO.Bind();
		VBO.Bind();
		VAO.LinkVBO(VBO, 0);
		EBO.Bind();

		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
	}
	else if (m_Type == "UV_SPHERE")
	{
		std::vector<float> vertices;
		std::vector<GLuint> indices;

		unsigned int stacks = 18;
		unsigned int slices = 36;
		float r = 0.5f;

		for (unsigned int i = 0; i <= stacks; ++i)
		{
			float theta = glm::pi<float>() * (float)i / (float)stacks;

			for (unsigned int j = 0; j <= slices; ++j)
			{
				float phi = 2.0f * glm::pi<float>() * (float)j / (float)slices;
				float x = r * glm::sin(theta) * glm::cos(phi);
				float y = r * glm::cos(theta);
				float z = r * glm::sin(theta) * glm::sin(phi);

				float u = (float)j / (float)slices;
				float v = (float)i / (float)stacks;

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
				vertices.push_back(1.0f);
				vertices.push_back(1.0f);
				vertices.push_back(1.0f);
				vertices.push_back(u);
				vertices.push_back(v);
			}
		}

		for (int i = 0; i <= stacks; ++i)
		{
			for (int j = 0; j <= slices; ++j)
			{
				int first = (i * (slices + 1)) + j;
				int second = first + slices + 1;

				// Two triangles per quad
				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);

				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}
		numVertices = indices.size();

		VAO VAO;
		VBO VBO((Vertex*)&vertices[0], vertices.size() * sizeof(float));
		EBO EBO(&indices[0], indices.size() * sizeof(GLuint));
		vaoID = VAO.ID;
		vboID = VBO.ID;
		eboID = EBO.ID;

		VAO.Bind();
		VBO.Bind();
		VAO.LinkVBO(VBO, 0);
		EBO.Bind();

		VAO.Unbind();
		VBO.Unbind();
		EBO.Unbind();
	}
}

std::vector<Vertex> Mesh::GetVertices() const
{
	return m_vertices;
}
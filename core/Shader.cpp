#include "config.h"

unsigned int Shader::create_module(const std::string& filepath, unsigned int module_type)
{
	std::ifstream file;
	std::stringstream bufferedLines;
	std::string line;

	file.open(filepath);
	while (std::getline(file, line))
	{
		bufferedLines << line << "\n";
	}

	std::string shaderSource = bufferedLines.str();
	const char* shaderSrc = shaderSource.c_str();
	file.close();

	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader Module compilation failed: \n" << errorLog << std::endl;
	}

	return shaderModule;
}

Shader::Shader(const char* vertex_filepath, const char* fragment_filepath)
{
	std::vector<unsigned int> modules;
	modules.push_back(create_module(vertex_filepath, GL_VERTEX_SHADER));
	modules.push_back(create_module(fragment_filepath, GL_FRAGMENT_SHADER));

	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules)
	{
		glAttachShader(shader, shaderModule);
	}

	glLinkProgram(shader);

	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking failed: \n" << errorLog << std::endl;
	}

	for (unsigned int shaderModule : modules)
	{
		glDeleteShader(shaderModule);
	}

	ID = shader;
}

Shader::Shader(std::vector<const char*> shader_plaintext)
{
	std::vector<unsigned int> modules;

	unsigned int vertexModule = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexModule, 1, &shader_plaintext[0], NULL);
	glCompileShader(vertexModule);

	int success;
	glGetShaderiv(vertexModule, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(vertexModule, 1024, NULL, errorLog);
		std::cout << "Vertex Shader Module compilation failed: \n" << errorLog << std::endl;
	}

	unsigned int fragmentModule = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentModule, 1, &shader_plaintext[1], NULL);
	glCompileShader(fragmentModule);

	glGetShaderiv(fragmentModule, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(fragmentModule, 1024, NULL, errorLog);
		std::cout << "Fragment Shader Module compilation failed: \n" << errorLog << std::endl;
	}

	modules.push_back(vertexModule);
	modules.push_back(fragmentModule);

	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules)
	{
		glAttachShader(shader, shaderModule);
	}

	glLinkProgram(shader);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking failed: \n" << errorLog << std::endl;
	}

	for (unsigned int shaderModule : modules)
	{
		glDeleteShader(shaderModule);
	}

	ID = shader;
}

void Shader::AddGeometryShader(const char* geometry_filepath)
{
	unsigned int geometryModule = create_module(geometry_filepath, GL_GEOMETRY_SHADER);
	glAttachShader(ID, geometryModule);
	glLinkProgram(ID);
	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(ID, 1024, NULL, errorLog);
		std::cout << "Geometry Shader linking failed: \n" << errorLog << std::endl;
	}
	glDeleteShader(geometryModule);
}

void Shader::AddGeometryShader(std::vector<const char*> geometry_plaintext)
{
	unsigned int geometryModule = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryModule, 1, &geometry_plaintext[0], NULL);
	glCompileShader(geometryModule);
	int success;
	glGetShaderiv(geometryModule, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(geometryModule, 1024, NULL, errorLog);
		std::cout << "Geometry Shader Module compilation failed: \n" << errorLog << std::endl;
	}

	glAttachShader(ID, geometryModule);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(ID, 1024, NULL, errorLog);
		std::cout << "Geometry Shader linking failed: \n" << errorLog << std::endl;
	}
	glDeleteShader(geometryModule);
}

void Shader::Activate() const
{
	glUseProgram(ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::SetVec3(const std::string& name, glm::vec3 vector) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z);
}
void Shader::AddTexture(Texture& texture)
{
	m_textures.push_back(texture);
	glUseProgram(ID);

	for (size_t i = 0; i < m_textures.size(); ++i)
    {
        std::string uniformName = "texture" + std::to_string(i);

        // Set the uniform to the corresponding texture unit
        glUniform1i(glGetUniformLocation(ID, uniformName.c_str()), i);

        // Activate and bind the texture to the texture unit
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].ID);
    }
}

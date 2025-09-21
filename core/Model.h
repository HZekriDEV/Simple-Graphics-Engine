#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "shader.h"
#include "mesh.h"

class Model
{
	public: 
		Model(const std::string& path, const Shader& shader = Shader("../OpenGL/shaders/default_vertex.vert", "../OpenGL/shaders/default_fragment.frag"), bool gamma = false);
		
		void SetShader(const Shader& shader);
		
		void Draw(const Camera& camera) const;

	private:
		// Model data
		std::vector<Mesh> m_meshes;
		std::string m_directory;
		std::vector<Texture> m_loadedTextures;
		Shader m_shader;
		bool gammaCorrection;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType type, std::string typeName);
		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};


#endif
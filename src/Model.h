#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include "Mesh.h"

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <string>

class Model
{
public:
	typedef struct MaterialUBO
	{
		int diffuseUsed;
		int specularUsed;
		int normalMapUsed;
		int heightMapUsed;
	}MaterialUBO;
	
	Model(const std::string& path);

	void draw(bool noTex, uint32_t materialUBOBindingPoint);
	std::vector<Mesh> meshes;
	std::vector<Mesh::Texture> texturesLoaded;
	
private:
	GLuint materialUBOId;
	
	void loadModel(const std::string &path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, Mesh::TexType texType);
	GLuint textureFromFile(const std::string &path, const std::string &directory, bool gamma = false);

	std::string directory;
};

#endif
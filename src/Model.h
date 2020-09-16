#ifndef MODEL_H
#define MODEL_H

#endif

#include "Shader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <string>

class Model
{
public:
	Model(const std::string& path);

	void draw(const Shader &shader, bool noTex);
	std::vector<Mesh> meshes;
	std::vector<Mesh::Texture> texturesLoaded;
	
private:
	void loadModel(const std::string &path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, Mesh::TexType texType);
	GLuint textureFromFile(const std::string &path, const std::string &directory, bool gamma = false);

	std::string directory;
};
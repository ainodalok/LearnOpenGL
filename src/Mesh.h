#ifndef MESH_H
#define MESH_H

class Model;

#include "Shader.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>

class Mesh
{
public:
	typedef struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	}Vertex;

	enum TexType
	{
		DIFFUSE,
		SPECULAR,
		NORMAL,
		HEIGHT
	};

	typedef struct Texture
	{
		GLuint id;
		TexType type;
		std::string path;
	}Texture;

	Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures);

	void draw(bool noTex, GLuint materialUBOId);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;


	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

private:

	void setupMesh();
};

#endif
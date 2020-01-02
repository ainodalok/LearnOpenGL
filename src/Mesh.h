#ifndef MESH_H
#define MESH_H

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
	}Vertex;

	enum TexType
	{
		DIFFUSE,
		SPECULAR
	};

	typedef struct Texture
	{
		GLuint id;
		TexType type;
		std::string path;
	}Texture;

	Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> indices, const std::vector<Texture> &textures);

	void draw(const Shader &shader);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	void setupMesh();
};

#endif
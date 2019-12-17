#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <stb_image.h>
#include <vector>

#include "Shader.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void render(bool wireframe = false);

private:
	GLfloat vertices[32] =
	{
		//positions				//colors			//texCoords
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f
	};

	GLuint indices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	std::vector<GLuint> textures;
	Shader* program;

	void loadTexture(const std::string &texturePath, GLint internalFormat, GLenum format);
};

#endif

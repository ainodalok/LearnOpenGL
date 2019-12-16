#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>

#include "Shader.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void render(bool wireframe = false);

private:
	GLfloat vertices[18] =
	{
		//positions				//colors
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	GLuint indices[3] =
	{
		0, 1, 2
	};

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	Shader* program;
};

#endif

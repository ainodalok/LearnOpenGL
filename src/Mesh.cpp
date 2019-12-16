#include "Mesh.h"

Mesh::Mesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	program = new Shader("Shaders/triangle.vert", "null", "Shaders/triangle.frag");
}

Mesh::~Mesh()
{
	delete program;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::render(bool wireframe)
{
	if(wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	program->use();
	float color0 = (sin(glfwGetTime() * 4.0f) + 1.0f) / 2.0f;
	float color1 = (sin((glfwGetTime() - 2.0f * glm::pi<float>() / 3.0f) * 4) + 1.0f) / 2.0f;
	float color2 = (sin((glfwGetTime() - 4.0f * glm::pi<float>() / 3.0f) * 4) + 1.0f) / 2.0f;
	glUniform1f(0, color0);
	glUniform1f(1, color1);
	glUniform1f(2, color2);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	if(wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

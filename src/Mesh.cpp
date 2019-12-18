#include "Mesh.h"

Mesh::Mesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	loadTexture("Textures/container.jpg", GL_RGB, GL_RGB);
	loadTexture("Textures/awesomeface.png", GL_RGB, GL_RGBA);

	program = new Shader("Shaders/triangle.vert", "null", "Shaders/triangle.frag");

	M = glm::mat4(1.0f);
}

Mesh::~Mesh()
{
	delete program;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}

void Mesh::loadTexture(const std::string &texturePath, GLint internalFormat, GLenum format)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	GLubyte* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Failed to load texture - " << texturePath << std::endl;
		exit(1);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	textures.push_back(texture);
}

void Mesh::render(const Camera& camera, bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	program->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBindVertexArray(VAO);

	glUniformMatrix4fv(1, 1, GL_FALSE, camera.getV());
	glUniformMatrix4fv(2, 1, GL_FALSE, camera.getP());

	for (unsigned int i = 0; i < 10; i++)
	{
		M = glm::mat4(1.0f);
		M = glm::translate(M, cubePositions[i]);
		M = glm::rotate(M, (float)glfwGetTime() * glm::radians(50.0f + 20 * i), glm::vec3(0.5f, 1.0f, 0.0f));
		M = glm::scale(M, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(M));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

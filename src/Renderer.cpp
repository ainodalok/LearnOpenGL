#include "Renderer.h"

Renderer::Renderer()
{
	//Object VAO
	VAOs.push_back(0);
	glGenVertexArrays(1, &VAOs[0]);
	glBindVertexArray(VAOs[0]);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float))); //Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //Texture coords
	glEnableVertexAttribArray(2);

	//Light VAO
	VAOs.push_back(0);
	glGenVertexArrays(1, &VAOs[1]);
	glBindVertexArray(VAOs[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	loadTexture("textures/container2.png", GL_RGB, GL_RGBA);
	glActiveTexture(GL_TEXTURE0 + 1);
	loadTexture("textures/container2_specular.png", GL_RGB, GL_RGBA);

	//Object program
	programs.push_back(0);
	programs[0] = new Shader("shaders/object.vert", "null", "shaders/object.frag");
	//Light program
	programs.push_back(0);
	programs[1] = new Shader("shaders/lamp.vert", "null", "shaders/lamp.frag");
}

Renderer::~Renderer()
{
	for (int i = 0; i < programs.size(); i++)
		delete programs[i];
	for (int i = 0; i < VAOs.size(); i++)
		glDeleteVertexArrays(1, &VAOs[i]);
	glDeleteBuffers(1, &VBO);
}

void Renderer::loadTexture(const std::string &texturePath, GLint internalFormat, GLenum format)
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

void Renderer::render(const Camera& camera, bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	
	glm::mat4 lightM = glm::rotate( static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));	
	lightM = glm::translate(lightM, lightPosition * 10.0f);

	//Object
	programs[0]->use();
	glBindVertexArray(VAOs[0]);
	glUniform1i(3, 0);	//Diffuse texture
	glUniform1i(4, 1);	//Specular texture
	glUniform1f(5, 32.0f);	//Specular object shininess
	//glUniform3fv(6, 1, glm::value_ptr(camera.getV() * lightM * glm::vec4(lightPosition, 1.0f)));	//viewspace position
	glUniform4fv(6, 1, glm::value_ptr(glm::vec4(glm::transpose(glm::inverse(glm::mat3(camera.getV() * lightM))) * -lightPosition, 0.0f)));	//light direction viewspace, scale independent
	glUniform3f(7, 0.2f, 0.2f, 0.2f);	//Ambient light color
	glUniform3f(8, 0.5f, 0.5f, 0.5f);	//Diffuse light color
	glUniform3f(9, 1.0f, 1.0f, 1.0f);	//Specualr light color

	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 objectM = glm::translate(cubePositions[i]);
		objectM = glm::rotate(objectM,  glm::radians(50.0f + 20 * i), glm::vec3(0.5f, 1.0f, 0.0f));
		
		glm::mat4 objectVM = camera.getV() * objectM;
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera.getP() * objectVM));	//PVM
		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(objectVM));	//VM
		glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(objectVM)))));	//transposedInvertedVM

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	
	lightM = glm::scale(lightM, glm::vec3(0.2f));

	//Light
	programs[1]->use();
	glBindVertexArray(VAOs[1]);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera.getP() * camera.getV() * lightM));	//PVM
	glDrawArrays(GL_TRIANGLES, 0, 36);

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

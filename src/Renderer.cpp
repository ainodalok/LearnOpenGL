#include "Renderer.h"

Renderer::Renderer()
{
	//VAO
	VAOs.push_back(0);
	glGenVertexArrays(1, &VAOs[0]);
	glBindVertexArray(VAOs[0]);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(planeVertices), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertices), cubeVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices), sizeof(planeVertices), planeVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	loadTexture("textures/marble.jpg");
	loadTexture("textures/metal.png");

	for (unsigned int i = 0; i < (sizeof(objectUBOs) / sizeof(ObjectUBO)); i++)
	{
		UBOs.push_back(0);
		glGenBuffers(1, &UBOs[i]);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[i]);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(ObjectUBO), nullptr, GL_STATIC_DRAW);
	}

	//Object program
	programs.push_back(0);
	programs[0] = new Shader("shaders/object.vert", "null", "shaders/object.frag");
	programs.push_back(0);
	programs[1] = new Shader("shaders/stencil.vert", "null", "shaders/stencil.frag");
}

Renderer::~Renderer()
{
	for (int i = 0; i < programs.size(); i++)
		delete programs[i];
	for (int i = 0; i < VAOs.size(); i++)
		glDeleteVertexArrays(1, &VAOs[i]);
	glDeleteBuffers(1, &VBO);
	for (int i = 0; i < textures.size(); i++)
		glDeleteTextures(1, &textures[i]);
}

void Renderer::loadTexture(const std::string &texturePath)
{
	int width, height, nrChannels;
	GLubyte* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Failed to load texture - " << texturePath << std::endl;
		stbi_image_free(data);
		return;
	}
	GLenum format;
	switch (nrChannels)
	{
	case 1:
		format = GL_RED;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		format = GL_RGB;
		break;
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	float aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	textures.push_back(texture);
}

void Renderer::render(Camera& camera, bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (camera.wasUpdatedP || camera.wasUpdatedV)
	{
		PV = camera.getP() * camera.getV();
		//glm::mat4 M;

		//M = glm::translate(glm::vec3(-1.0f, 0.0f, -1.0f));
		//objectUBOs[0].PVM = PV * M;
		//M = glm::translate(glm::vec3(2.0f, 0.0f, 0.0f));
		//objectUBOs[1].PVM = PV * M;
		objectUBOs[2].PVM = PV;
		//for (unsigned int i = 0; i < (sizeof(objectUBOs) / sizeof(ObjectUBO)); i++)
		//{
			glBindBuffer(GL_UNIFORM_BUFFER, UBOs[2]);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ObjectUBO), &objectUBOs[2]);
		//}
	}

	glm::mat4 M;

	glDepthMask(GL_FALSE);
	programs[0]->use();
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1000, 0xFF);
	glStencilMask(0xFF);
	M = glm::scale(glm::vec3(3.0f, 3.0f, 3.0f));
	objectUBOs[2].PVM = PV * M;
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ObjectUBO), &objectUBOs[2]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[2], 0, sizeof(ObjectUBO));
	glDrawArrays(GL_TRIANGLES, sizeof(cubeVertices) / sizeof(float) / 5, sizeof(planeVertices) / sizeof(float) / 5);
	glDepthMask(GL_TRUE);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	programs[0]->use();
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glStencilFunc(GL_GEQUAL, 1, 0xFF);
	glStencilMask(0xFF);
	M = glm::translate(glm::vec3(-1.0f, 0.5f, -1.0f));
	objectUBOs[0].PVM = PV * M;
	glBindBuffer(GL_UNIFORM_BUFFER, UBOs[0]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ObjectUBO), &objectUBOs[0]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[0], 0, sizeof(ObjectUBO));
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float) / 5);

	programs[1]->use();
	glStencilFunc(GL_GREATER, 1, 0xFF);
	glStencilMask(0x00);
	M = glm::translate(glm::vec3(-1.0f, 0.5f, -1.0f));
	M = glm::scale(M, glm::vec3(1.02f, 1.02f, 1.02f));
	objectUBOs[0].PVM = PV * M;
	glBindBuffer(GL_UNIFORM_BUFFER, UBOs[0]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ObjectUBO), &objectUBOs[0]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[0], 0, sizeof(ObjectUBO));
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float) / 5);

	programs[0]->use();
	glStencilFunc(GL_GEQUAL, 2, 0xFF);
	glStencilMask(0xFF);
	M = glm::translate(glm::vec3(2.0f, 0.5f, 0.0f));
	objectUBOs[1].PVM = PV * M;
	glBindBuffer(GL_UNIFORM_BUFFER, UBOs[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ObjectUBO), &objectUBOs[1]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[1], 0, sizeof(ObjectUBO));
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float) / 5);

	programs[1]->use();
	glStencilFunc(GL_GREATER, 2, 0xFF);
	glStencilMask(0x00);
	M = glm::translate(glm::vec3(2.0f, 0.5f, 0.0f));
	M = glm::scale(M, glm::vec3(1.202f, 1.202f, 1.202f));
	objectUBOs[1].PVM = PV * M;
	glBindBuffer(GL_UNIFORM_BUFFER, UBOs[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ObjectUBO), &objectUBOs[1]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[1], 0, sizeof(ObjectUBO));
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float) / 5);
	
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glStencilMask(0xFF);



	if (camera.wasUpdatedP || camera.wasUpdatedV)
	{
		camera.wasUpdatedP = false;
		camera.wasUpdatedV = false;
	}

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

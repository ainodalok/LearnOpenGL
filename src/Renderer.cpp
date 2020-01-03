#include "Renderer.h"

Renderer::Renderer()
{
	//Cube VAO
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

	//Plane VAO
	VAOs.push_back(0);
	glGenVertexArrays(1, &VAOs[1]);
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(cubeVertices) + 0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(cubeVertices) + 3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	loadTexture("textures/marble.jpg");
	loadTexture("textures/metal.png");

	int alignment = 0;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);
	objectUBOOffset = sizeof(ObjectUBO) + (alignment - sizeof(ObjectUBO) % alignment);

	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, objectUBOOffset * (sizeof(objectUBOs) / sizeof(ObjectUBO)), nullptr, GL_STATIC_DRAW);

	//Object program
	programs.push_back(0);
	programs[0] = new Shader("shaders/object.vert", "null", "shaders/object.frag");
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
		glm::mat4 M;

		M = glm::translate(glm::vec3(-1.0f, 0.0f, -1.0f));
		objectUBOs[0].PVM = PV * M;
		M = glm::translate(glm::vec3(2.0f, 0.0f, 0.0f));
		objectUBOs[1].PVM = PV * M;
		objectUBOs[2].PVM = PV;
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		void* ptr = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		for (unsigned long long i = 0; i < (sizeof(objectUBOs) / sizeof(ObjectUBO)); i++)
			memcpy((static_cast<unsigned char*>(ptr) + objectUBOOffset * i), &objectUBOs[i], sizeof(ObjectUBO));
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}

	programs[0]->use();
	glBindVertexArray(VAOs[0]);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, objectUBOOffset * i, sizeof(ObjectUBO));
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float) / 5);
	}

	glBindVertexArray(VAOs[1]);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, objectUBOOffset * 2, sizeof(ObjectUBO));
	glDrawArrays(GL_TRIANGLES, 0, sizeof(planeVertices) / sizeof(float) / 5);

	if (camera.wasUpdatedP || camera.wasUpdatedV)
	{
		camera.wasUpdatedP = false;
		camera.wasUpdatedV = false;
	}

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

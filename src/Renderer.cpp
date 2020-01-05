#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
	//VAO
	VAOs.push_back(0);
	glGenVertexArrays(1, &VAOs[0]);
	glBindVertexArray(VAOs[0]);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(planeVertices) + sizeof(transparentVertices), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertices), cubeVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices), sizeof(planeVertices), planeVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(planeVertices), sizeof(transparentVertices), transparentVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	loadTexture("textures/marble.jpg", GL_REPEAT);
	loadTexture("textures/metal.png", GL_REPEAT);
	loadTexture("textures/blending_transparent_window.png", GL_CLAMP_TO_EDGE);

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
	programs[1] = new Shader("shaders/screen.vert", "null", "shaders/screen.frag");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	rebuildFramebuffer(width, height);
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
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &FBOtexture);
	glDeleteFramebuffers(1, &FBO);
}

void Renderer::loadTexture(const std::string &texturePath, GLint wrapMode)
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
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
		M = glm::translate(glm::vec3(0.0f, -0.5f, 0.0f));
		M = glm::scale(M, glm::vec3(3.0f, 3.0f, 3.0f));
		objectUBOs[2].PVM = PV * M;
		std::map<float, glm::vec3> sortedWindows;
		for (unsigned int i = 0; i < vegetation.size(); i++)
		{
			float distance = glm::length(camera.pos - vegetation[i]);
			sortedWindows[distance] = vegetation[i];
		}
		int i = 0;
		for (std::map<float, glm::vec3>::reverse_iterator it = sortedWindows.rbegin(); it != sortedWindows.rend(); it++, i++)
		{
			M = glm::translate(it->second);
			objectUBOs[i + 3].PVM = PV * M;
		}
		for (unsigned int i = 0; i < UBOs.size(); i++)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, UBOs[i]);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ObjectUBO), &objectUBOs[i]);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	programs[0]->use();
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[2], 0, sizeof(ObjectUBO));
	glDrawArrays(GL_TRIANGLES, sizeof(cubeVertices) / sizeof(float) / 5, sizeof(planeVertices) / sizeof(float) / 5);

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[0], 0, sizeof(ObjectUBO));
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float) / 5);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[1], 0, sizeof(ObjectUBO));
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float) / 5);

	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	for (unsigned int i = 0; i < vegetation.size(); i++)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[i + 3], 0, sizeof(ObjectUBO));
		glDrawArrays(GL_TRIANGLES, (sizeof(cubeVertices) + sizeof(planeVertices)) / (sizeof(float) * 5), sizeof(transparentVertices) / (sizeof(float) * 5));
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	programs[1]->use();
	glBindTexture(GL_TEXTURE_2D, FBOtexture);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glEnable(GL_DEPTH_TEST);

	if (camera.wasUpdatedP || camera.wasUpdatedV)
	{
		camera.wasUpdatedP = false;
		camera.wasUpdatedV = false;
	}

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::rebuildFramebuffer(int width, int height)
{
	if (FBO != 0)
	{
		glDeleteTextures(1, &FBOtexture);
		glDeleteRenderbuffers(1, &RBO);
	}
	else
	{
		glGenFramebuffers(1, &FBO);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenTextures(1, &FBOtexture);
	glBindTexture(GL_TEXTURE_2D, FBOtexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtexture, 0);
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

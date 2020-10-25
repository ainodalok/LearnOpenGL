#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
	//Static data construction and transfer
	GLuint VAO;
	GLuint VBO;
	GLuint UBO;

	glGenVertexArrays(1, &VAO);
	VAOs.push_back(VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	VBOs.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float cubeVertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right      
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right	
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// left face
		-1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		// right face
		 1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right  
		 1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left 
		 1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-right
		-1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-right  
		 1.0f,  1.0f , 1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-left 
		-1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f  // top-left   
	};
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	
	//cameraUBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PVUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	//modelUBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(MUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);
	
	//lightUBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	programs.emplace_back("shaders/screen.vert", "null", "shaders/hdr.frag");
	programs.emplace_back("shaders/litHDRObj.vert", "null", "shaders/litHDRObj.frag");

	load2DTexture("textures/wood.png", GL_REPEAT, true);
		
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	rebuildScreenFramebuffers(width, height);
	
	glEnable(GL_PROGRAM_POINT_SIZE);
}

Renderer::~Renderer()
{
	for (int i = 0; i < UBOs.size(); i++)
		glDeleteBuffers(1, &UBOs[i]);
	for (int i = 0; i < VAOs.size(); i++)
		glDeleteVertexArrays(1, &VAOs[i]);
	for (int i = 0; i < VBOs.size(); i++)
		glDeleteBuffers(1, &VBOs[i]);
	for (int i = 0; i < textures.size(); i++)
		glDeleteTextures(1, &textures[i]);
	glDeleteRenderbuffers(1, &RBOScreenMSAA);
	glDeleteTextures(1, &TextureScreenMSAA);
	glDeleteFramebuffers(1, &FBOScreenMSAA);
	glDeleteRenderbuffers(1, &RBOScreen);
	glDeleteTextures(1, &TextureScreen);
	glDeleteFramebuffers(1, &FBOScreen);
}

void Renderer::load2DTexture(const std::string &texturePath, GLint wrapMode, bool srgb)
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
	GLenum intFormat;
	switch (nrChannels)
	{
	case 1:
		intFormat = GL_RED;
		format = GL_RED;
		break;
	case 2:
		intFormat = GL_RG;
		format = GL_RG;
		break;
	case 3:
		intFormat = srgb ? GL_SRGB : GL_RGB;
		format = GL_RGB;
		break;
	case 4:
		intFormat = srgb ? GL_SRGB_ALPHA : GL_RGBA;
		format = GL_RGBA;
		break;
	default:
		intFormat = GL_RGB;
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
	glTexImage2D(GL_TEXTURE_2D, 0, intFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	textures.push_back(texture);
}

void Renderer::loadCubeMap(const std::vector<std::string> &texturePaths)
{
	//Cubemap's texture origin is inverted
	stbi_set_flip_vertically_on_load(false);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	int width, height, nrChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < texturePaths.size(); i++)
	{
		data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			std::cout << "Cubemap tex failed to load at path: " << texturePaths[i] << std::endl;
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//Clamp to edge so that when sampling exactly between two faces there would always be edge data
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	textures.push_back(texture);
	stbi_set_flip_vertically_on_load(true);
}

void Renderer::buildDepthMapFramebuffer(GLuint &FBO, GLuint &texture)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glGenTextures(1, &depthMapTexture);
	//glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapTexture, 0);
	//Since no color attachments
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void Renderer::updateUniforms(Camera& camera)
{
	//UBOs - cameraUBO, objectUBO, lightUBO
	//Update matrices and UBOs here
	if (camera.wasUpdatedP || camera.wasUpdatedV)
	{
		cameraUBO.PV = camera.getP() * camera.getV();
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(cameraUBO), &cameraUBO);

		lightUBO.lightPos[0] = glm::vec4(0.0f, 0.0f, 49.5f, 0.0f);
		lightUBO.lightPos[1] = glm::vec4(-1.4f, -1.9f, 9.0f, 0.0f);
		lightUBO.lightPos[2] = glm::vec4(0.0f, -1.8f, 4.0f, 0.0f);
		lightUBO.lightPos[3] = glm::vec4(0.8f, -1.7f, 6.0f, 0.0f);
		lightUBO.lightCol[0] = glm::vec4(200.0f, 200.0f, 200.0f, 0.0f);
		lightUBO.lightCol[1] = glm::vec4(0.1f, 0.0f, 0.0f, 0.0f);
		lightUBO.lightCol[2] = glm::vec4(0.0f, 0.0f, 0.2f, 0.0f);
		lightUBO.lightCol[3] = glm::vec4(0.0f, 0.1f, 0.0f, 0.0f);
		lightUBO.viewPos = glm::vec4(camera.pos, 1.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[2]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightUBO), &lightUBO);
	}

	modelUBO.M = glm::mat4(1.0f);
	modelUBO.M = glm::translate(modelUBO.M, glm::vec3(0.0f, 0.0f, 25.0f));
	//modelUBO.M = glm::rotate(modelUBO.M, glm::radians(static_cast<float>(glfwGetTime()) * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	modelUBO.M = glm::scale(modelUBO.M, glm::vec3(2.5f, 2.5f, 27.5f));
	modelUBO.cofactorM = glm::transpose(glm::adjugate(glm::mat3(modelUBO.M)));
	glBindBuffer(GL_UNIFORM_BUFFER, UBOs[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(modelUBO), &modelUBO);
}

void Renderer::renderScene()
{
	programs[1].use();
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBOs[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, UBOs[1]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, UBOs[2]);
	glBindVertexArray(VAOs[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::render(Camera &camera, bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	updateUniforms(camera);
	
	glBindFramebuffer(GL_FRAMEBUFFER, FBOScreenMSAA);
	glViewport(0, 0, this->width, this->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	renderScene();

	//Resolve MSAA
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBOScreenMSAA);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBOScreen);
	glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

	//Render to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	programs[0].use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureScreen);
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

void Renderer::rebuildScreenFramebuffers(int width, int height)
{
	this->width = width;
	this->height = height;
	if (FBOScreenMSAA != 0)
	{
		glDeleteTextures(1, &TextureScreenMSAA);
		glDeleteRenderbuffers(1, &RBOScreenMSAA);
	}
	else
		glGenFramebuffers(1, &FBOScreenMSAA);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOScreenMSAA);
	glGenTextures(1, &TextureScreenMSAA);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, TextureScreenMSAA);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, width, height, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, TextureScreenMSAA, 0);
	glGenRenderbuffers(1, &RBOScreenMSAA);
	glBindRenderbuffer(GL_RENDERBUFFER, RBOScreenMSAA);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOScreenMSAA);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::FRAMEBUFFER:: MSAA Screen Framebuffer is not complete!" << std::endl;

	if (FBOScreen != 0)
	{
		glDeleteTextures(1, &TextureScreen);
		glDeleteRenderbuffers(1, &RBOScreen);
	}
	else
		glGenFramebuffers(1, &FBOScreen);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOScreen);
	glGenTextures(1, &TextureScreen);
	glBindTexture(GL_TEXTURE_2D, TextureScreen);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureScreen, 0);
	glGenRenderbuffers(1, &RBOScreen);
	glBindRenderbuffer(GL_RENDERBUFFER, RBOScreen);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBOScreen);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::FRAMEBUFFER:: Resolved Screen Framebuffer is not complete!" << std::endl;
}

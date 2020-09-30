#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
	//Static data construction and transfer
	GLuint VAO;
	GLuint VBO;
	GLuint UBO;

	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f
	};

	float cubeVertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	VAOs.push_back(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	VBOs.push_back(VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices) + sizeof(cubeVertices), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(planeVertices), planeVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(planeVertices), sizeof(cubeVertices), cubeVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Camera
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PVUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);
	
	//Point light
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	//Omni-Directional light shadow map
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	float nearPlane = 1.0f;
	float aspect = static_cast<float>(SHADOW_WIDTH) / static_cast<float>(SHADOW_HEIGHT);
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, nearPlane, shadowFarPlane);
	std::vector<glm::mat4> shadowTransforms;
	omniShadowUBO.shadowPV[0] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	omniShadowUBO.shadowPV[1] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
	omniShadowUBO.shadowPV[2] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
	omniShadowUBO.shadowPV[3] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
	omniShadowUBO.shadowPV[4] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
	omniShadowUBO.shadowPV[5] = shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));
	glBufferData(GL_UNIFORM_BUFFER, sizeof(OmniShadowUBO), &omniShadowUBO, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	//Floor
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	floorUBO.M = glm::identity<glm::mat4>();
	floorUBO.cofactorM = floorUBO.M;
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ObjectBlock), &floorUBO, GL_STATIC_DRAW);
	UBOs.push_back(UBO);
	
	//3 Cubes
	cubeUBOs[0].M = glm::translate(glm::vec3(0.0f, 1.5f, 0.0)) * 
					glm::scale(glm::vec3(0.5f));
	cubeUBOs[1].M = glm::translate(glm::vec3(2.0f, 0.0f, 1.0)) * 
					glm::scale(glm::vec3(0.5f));
	cubeUBOs[2].M = glm::translate(glm::vec3(-1.0f, 0.0f, 2.0)) *
					glm::rotate(glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))) * 
					glm::scale(glm::vec3(0.25f));
	for (int i = 0; i < sizeof(cubeUBOs) / sizeof(ObjectBlock); i++)
	{
		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		cubeUBOs[i].cofactorM = glm::transpose(glm::adjugate(glm::mat3(cubeUBOs[i].M)));
		glBufferData(GL_UNIFORM_BUFFER, sizeof(ObjectBlock), &cubeUBOs[i], GL_STATIC_DRAW);
		UBOs.push_back(UBO);
	}
	
	load2DTexture("textures/wood.png", GL_REPEAT, true);

	programs.emplace_back("shaders/litOmniShadowObj.vert", "null", "shaders/litOmniShadowObj.frag");
	programs.emplace_back("shaders/omniDepth.vert", "shaders/omniDepth.geom", "shaders/omniDepth.frag");
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	rebuildFramebuffer(width, height);

	buildDepthMapFramebuffer(depthMapFBO, depthMapTexture);
	
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
	glDeleteTextures(1, &depthMapTexture);
	glDeleteFramebuffers(1, &depthMapFBO);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &FBOtexture);
	glDeleteFramebuffers(1, &FBO);
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
	//Update matrices and UBOs here
	if (camera.wasUpdatedP || camera.wasUpdatedV)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[0]);
		cameraUBO.PV = camera.getP() * camera.getV();
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PVUBO), &cameraUBO);

		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[1]);
		lightUBO.lightPos = lightPos;
		lightUBO.viewPos = glm::vec4(camera.pos, 1.0f);
		lightUBO.farPlane = shadowFarPlane;
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(LightUBO), &lightUBO);
	}
}

void Renderer::renderScene(bool shadow)
{
	//UBOs - cameraUBO, lightUBO, omniDirectShadowUBO, floorUBO, cubeUBOs[3]
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, UBOs[1]); // lightUBO
	if (shadow)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, UBOs[2]); // omniDirectShadowUBO
		programs[1].use();
		//glCullFace(GL_FRONT);
	}
	
	//Render data on GPU
	if (!shadow)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, UBOs[0]); // cameraUBO
		programs[0].use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapTexture);
	}
	glBindVertexArray(VAOs[0]);
	//Floor
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBOs[3]); // floorUBO
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	//Cubes
	for (int i = 0; i < sizeof(cubeUBOs) / sizeof(ObjectBlock); i++)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBOs[4 + i]); // cubeUBOs
		glDrawArrays(GL_TRIANGLES, 6, 36);
	}

	//if (shadow) glCullFace(GL_BACK);
}

void Renderer::render(Camera &camera, bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	updateUniforms(camera);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	renderScene(true);
	
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, this->width, this->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	renderScene(false);
	
	//Blit to the screen
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST);
	//programs[1].use();
	//glBindTexture(GL_TEXTURE_2D, FBOtexture);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glEnable(GL_DEPTH_TEST);
	glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

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
	this->width = width;
	this->height = height;
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
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, FBOtexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, FBOtexture, 0);
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

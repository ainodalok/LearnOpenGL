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
	// positions
	glm::vec3 pos1(-1.0, 1.0, 0.0);
	glm::vec3 pos2(-1.0, -1.0, 0.0);
	glm::vec3 pos3(1.0, -1.0, 0.0);
	glm::vec3 pos4(1.0, 1.0, 0.0);
	// texture coordinates
	glm::vec2 uv1(0.0, 1.0);
	glm::vec2 uv2(0.0, 0.0);
	glm::vec2 uv3(1.0, 0.0);
	glm::vec2 uv4(1.0, 1.0);
	// normal vector
	glm::vec3 nm(0.0, 0.0, 1.0);

	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;
	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	glm::mat2 inverseUVs(deltaUV2.y, -deltaUV2.x, -deltaUV1.y, deltaUV1.x);
	glm::mat3x2 edges;
	edges = glm::row(edges, 0, edge1);
	edges = glm::row(edges, 1, edge2);
	glm::mat3x2 TB1 = f * inverseUVs * edges;
	glm::vec3 tangent1 = glm::row(TB1, 0);
	glm::vec3 bitangent1 = glm::row(TB1, 1);

	edge1 = pos4 - pos1;
	deltaUV1 = uv4 - uv1;
	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
	inverseUVs = glm::mat2(deltaUV2.y, -deltaUV2.x, -deltaUV1.y, deltaUV1.x);
	edges = glm::row(edges, 0, edge1);
	glm::mat3x2 TB2 = f * inverseUVs * edges;
	glm::vec3 tangent2 = glm::row(TB2, 0);
	glm::vec3 bitangent2 = glm::row(TB2, 1);

	float planeVertices[] =
	{ pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
	  pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
	  pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
	  pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
	  pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
	  pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z };

	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	
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
	
	programs.emplace_back("shaders/litNormalMap.vert", "null", "shaders/litParallaxMap.frag");

	load2DTexture("textures/bricks2.jpg", GL_REPEAT, true);
	load2DTexture("textures/bricks2_normal.jpg", GL_REPEAT, false);
	load2DTexture("textures/bricks2_disp.jpg", GL_REPEAT, false);
		
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	rebuildFramebuffer(width, height);
	
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
	//UBOs - cameraUBO, objectUBO, lightUBO
	//Update matrices and UBOs here
	if (camera.wasUpdatedP || camera.wasUpdatedV)
	{
		cameraUBO.PV = camera.getP() * camera.getV();
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(cameraUBO), &cameraUBO);

		lightUBO.lightPos = glm::vec3(0.5f, 1.0f, 0.3f);
		lightUBO.viewPos = glm::vec4(camera.pos, 1.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[2]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(lightUBO), &lightUBO);
	}

	modelUBO.M = glm::mat4(1.0f);
	//modelUBO.M = glm::translate(modelUBO.M, glm::vec3(0.0f, 0.0f, -1.0f));
	//modelUBO.M = glm::rotate(modelUBO.M, glm::radians(static_cast<float>(glfwGetTime()) * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	modelUBO.cofactorM = glm::transpose(glm::adjugate(glm::mat3(modelUBO.M)));
	glBindBuffer(GL_UNIFORM_BUFFER, UBOs[1]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(modelUBO), &modelUBO);
}

void Renderer::renderScene()
{
	programs[0].use();
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBOs[0]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, UBOs[1]);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, UBOs[2]);
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("Parallax Debug", (bool*)0, overlayBox);
	ImGui::SliderFloat("Height Scale", &heightScale, 0.001f, 1.0f);
	ImGui::End();
	glUniform1f(0, heightScale);
	glBindVertexArray(VAOs[0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::render(Camera &camera, bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	updateUniforms(camera);
	
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, this->width, this->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	renderScene();
	
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

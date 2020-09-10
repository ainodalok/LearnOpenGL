#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
	//Meshes
	GLuint VAO;
	GLuint VBO;
	
	glGenVertexArrays(1, &VAO);
	VAOs.push_back(VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	VBOs.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(skyboxVertices), skyboxVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	
	glGenVertexArrays(1, &VAO);
	VAOs.push_back(VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	VBOs.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(planeVertices) + sizeof(transparentVertices), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertices), cubeVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices), sizeof(planeVertices), planeVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(planeVertices), sizeof(transparentVertices), transparentVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &VAO);
	VAOs.push_back(VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	VBOs.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeRefractVertices), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeRefractVertices), cubeRefractVertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//Textures
	load2DTexture("textures/marble.jpg", GL_REPEAT);
	load2DTexture("textures/wall.jpg", GL_REPEAT);
	load2DTexture("textures/blending_transparent_window.png", GL_CLAMP_TO_EDGE);
	loadCubeMap(texturePaths);

	//UBOs
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(FloorUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SkyboxUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ReflectUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	for (unsigned int i = 0; i < (sizeof(objectUBOs) / sizeof(ObjectUBO)); i++)
	{
		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(ObjectUBO), nullptr, GL_STATIC_DRAW);
		UBOs.push_back(UBO);
	}

	//Shaders
	programs.push_back(new Shader("shaders/object.vert", "null", "shaders/object.frag"));
	programs.push_back(new Shader("shaders/screen.vert", "null", "shaders/screen.frag"));
	programs.push_back(new Shader("shaders/floor.vert", "null", "shaders/object.frag"));
	programs.push_back(new Shader("shaders/skybox.vert", "null", "shaders/skybox.frag"));
	programs.push_back(new Shader("shaders/refract.vert", "null", "shaders/refract.frag"));

	nanosuit = new Model("models/nanosuit/nanosuit.obj");
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	rebuildFramebuffer(width, height);
}

Renderer::~Renderer()
{
	for (int i = 0; i < UBOs.size(); i++)
		glDeleteBuffers(1, &UBOs[i]);
	for (int i = 0; i < programs.size(); i++)
		delete programs[i];
	for (int i = 0; i < VAOs.size(); i++)
		glDeleteVertexArrays(1, &VAOs[i]);
	for (int i = 0; i < VBOs.size(); i++)
		glDeleteBuffers(1, &VBOs[i]);
	for (int i = 0; i < textures.size(); i++)
		glDeleteTextures(1, &textures[i]);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &FBOtexture);
	glDeleteFramebuffers(1, &FBO);
	delete nanosuit;
}

void Renderer::load2DTexture(const std::string &texturePath, GLint wrapMode)
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

void Renderer::render(Camera &camera, bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (camera.wasUpdatedP || camera.wasUpdatedV)
	{
		PV = camera.getP() * camera.getV();

		//Construct UBOs
		glm::mat4 M;
		//Floor
		M = glm::translate(glm::vec3(camera.pos.x, -0.5f, camera.pos.z));
		//M = glm::scale(M, glm::vec3(3.0f, 3.0f, 3.0f));
		floorUBO.PVM = PV * M;
		floorUBO.pos = camera.pos;

		//Skybox
		skyboxUBO.PV = camera.getP() * glm::mat4(glm::mat3(camera.getV()));
		
		//Cube (object)
		M = glm::translate(glm::vec3(-1.0f, 0.0f, -1.0f));
		objectUBOs[0].PVM = PV * M;
		
		//Windows (objects)
		//Sort by distance from camera
		std::map<float, glm::vec3> sortedWindows;
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length(camera.pos - windows[i]);
			sortedWindows[distance] = windows[i];
		}
		//and construct UBOs for each of them
		int i = 1;
		for (std::map<float, glm::vec3>::reverse_iterator it = sortedWindows.rbegin(); it != sortedWindows.rend(); it++, i++)
		{
			M = glm::translate(it->second);
			objectUBOs[i].PVM = PV * M;
		}

		M = glm::translate(glm::vec3(2.0f, 0.0f, 0.0f));
		refractUBO.PVM = PV * M;
		refractUBO.M = M;
		refractUBO.transposedInvertedM = glm::transpose(glm::inverse(refractUBO.M));
		refractUBO.camera = camera.pos;

		//Send UBO data to GPU
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(FloorUBO), &floorUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[1]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SkyboxUBO), &skyboxUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[2]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ReflectUBO), &refractUBO);
		for (unsigned int i = 0; i < sizeof(objectUBOs) / sizeof(ObjectUBO); i++)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, UBOs[i + 3]);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ObjectUBO), &objectUBOs[i]);
		}
	}

	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("Internals", (bool*)0, overlayBox);
	ImGui::Text("%f, %f, %f camera pos", camera.pos.x, camera.pos.y, camera.pos.z);
	ImGui::Text("TIM - %s", glm::to_string(refractUBO.transposedInvertedM).c_str());
	ImGui::Text("M - %s", glm::to_string(refractUBO.M).c_str());
	ImGui::Text("PVM - %s", glm::to_string(refractUBO.PVM).c_str());
	ImGui::End();

	
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindVertexArray(VAOs[1]);
	
	//Render floor
	programs[2]->use();
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[0], 0, sizeof(FloorUBO));
	glDrawArrays(GL_TRIANGLES, sizeof(cubeVertices) / sizeof(float) / 5, sizeof(planeVertices) / sizeof(float) / 5);

	//Render cube
	programs[0]->use();
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[3], 0, sizeof(ObjectUBO));
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / sizeof(float) / 5);

	//Render reflective cube
	glBindVertexArray(VAOs[2]);
	programs[4]->use();
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[3]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[2], 0, sizeof(ReflectUBO));
	nanosuit->draw(*programs[4], true);
	//glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeRefractVertices) / sizeof(float) / 6);
	
	//Render skybox
	glBindVertexArray(VAOs[0]);
	glDepthFunc(GL_LEQUAL);
	programs[3]->use();
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[1], 0, sizeof(SkyboxUBO));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
	
	glBindVertexArray(VAOs[1]);
	//Render each window
	programs[0]->use();
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	for (unsigned int i = 0; i < windows.size(); i++)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[i + 4], 0, sizeof(ObjectUBO));
		glDrawArrays(GL_TRIANGLES, (sizeof(cubeVertices) + sizeof(planeVertices)) / (sizeof(float) * 5), sizeof(transparentVertices) / (sizeof(float) * 5));
	}
	
	//Blit to the screen
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

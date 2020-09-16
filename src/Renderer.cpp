#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
	//Meshes
	GLuint VAO;
	GLuint VBO;

	//Shaders
	programs.emplace_back("shaders/object.vert", "null", "shaders/object.frag");
	programs.emplace_back("shaders/screen.vert", "null", "shaders/screen.frag");
	programs.emplace_back("shaders/floor.vert", "null", "shaders/object.frag");
	programs.emplace_back("shaders/skybox.vert", "null", "shaders/skybox.frag");
	programs.emplace_back("shaders/refract.vert", "null", "shaders/refract.frag");
	programs.emplace_back("shaders/geometry.vert", "shaders/geometry.geom", "shaders/geometry.frag");
	programs.emplace_back("shaders/explode.vert", "shaders/explode.geom", "shaders/explode.frag");
	programs.emplace_back("shaders/model.vert", "null", "shaders/object.frag");
	programs.emplace_back("shaders/instanced.vert", "null", "shaders/object.frag");

	models.emplace_back("models/nanosuit/nanosuit.obj");
	models.emplace_back("models/planet/planet.obj");
	models.emplace_back("models/rock/rock.obj");

	asteroidMatrices = new glm::mat4[amount];
	randAsteroidRotationVec = new glm::vec3[amount];

	//Skybox
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

	//Cube+Floor+Window
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

	//Refraction Cube
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

	//Points
	glGenVertexArrays(1, &VAO);
	VAOs.push_back(VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	VBOs.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Addition to Asteroid VAO
	//Create model matrix for each asteroid in the ring
	srand(glfwGetTime()); // initialize random seed	
	float radius = 12.5;
	float offset = 2.5f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		float angle = static_cast<float>(i) / static_cast<float>(amount) * 360.0f;
		float displacement = (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		float scale = (rand() % 625) / 10000.0f + 0.0125;
		model = glm::scale(model, glm::vec3(scale));
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		asteroidMatrices[i] = model;
		randAsteroidRotationVec[i] = glm::sphericalRand(1.0f);
	}
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &asteroidMatrices[0], GL_STATIC_DRAW);
	for (int i = 0; i < models[2].meshes.size(); i++)
	{
		glBindVertexArray(models[2].meshes[i].VAO);
		std::size_t vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
	}
	
	//Textures
	load2DTexture("textures/marble.jpg", GL_REPEAT);
	load2DTexture("textures/wall.jpg", GL_REPEAT);
	load2DTexture("textures/blending_transparent_window.png", GL_CLAMP_TO_EDGE);
	loadCubeMap(texturePaths);

	//UBOs
	//Floor UBO
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PVMposUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	//Skybox UBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PVUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	//Refract UBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(RefractUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	//Asteroid UBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PVMrotUBO), nullptr, GL_STATIC_DRAW);
	UBOs.push_back(UBO);

	//One cube, planet and windows
	objectUBOs.resize(windows.size() + 2);
	for (unsigned int i = 0; i < objectUBOs.size(); i++)
	{
		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(PVMUBO), nullptr, GL_STATIC_DRAW);
		UBOs.push_back(UBO);
	}
	
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
	delete[] asteroidMatrices;
	delete[] randAsteroidRotationVec;
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &FBOtexture);
	glDeleteFramebuffers(1, &FBO);
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

		//Refractive object
		M = glm::scale(glm::vec3(0.1f, 0.1f, 0.1f));
		M = glm::translate(M, glm::vec3(2.0f, -5.0f, 0.0f));
		refractUBO.PV = PV;
		refractUBO.M = M;
		refractUBO.cofactorM = glm::transpose(glm::adjugate(refractUBO.M));
		refractUBO.camera = camera.pos;
		refractUBO.time = 0.0f;

		//Cube (object)
		M = glm::translate(glm::vec3(-1.0f, 0.0f, -1.0f));
		objectUBOs[0].PVM = PV * M;

		//Planet (model)
		M = glm::translate(glm::vec3(0.0f, 20.0f, -10.0f));
		M = glm::scale(M, glm::vec3(1.0f));
		objectUBOs[1].PVM = PV * M;
		
		//Windows (objects)
		//Sort by distance from camera
		std::map<float, glm::vec3> sortedWindows;
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length(camera.pos - windows[i]);
			sortedWindows[distance] = windows[i];
		}
		//and construct UBOs for each of them
		int i = 2;
		for (std::map<float, glm::vec3>::reverse_iterator it = sortedWindows.rbegin(); it != sortedWindows.rend(); it++, i++)
		{
			M = glm::translate(it->second);
			objectUBOs[i].PVM = PV * M;
		}

		//Send UBO data to GPU
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[0]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PVMposUBO), &floorUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[1]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PVUBO), &skyboxUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOs[2]);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(RefractUBO), &refractUBO);
		for (unsigned int i = 0; i < objectUBOs.size(); i++)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, UBOs[i + UBOs.size() - objectUBOs.size()]);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PVMUBO), &objectUBOs[i]);
		}
	}

	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("Internals", (bool*)0, overlayBox);
	ImGui::Text("%f, %f, %f camera pos", camera.pos.x, camera.pos.y, camera.pos.z);
	ImGui::Text("TIM - %s", glm::to_string(refractUBO.cofactorM).c_str());
	ImGui::Text("M - %s", glm::to_string(refractUBO.M).c_str());
	ImGui::Text("PV - %s", glm::to_string(refractUBO.PV).c_str());
	ImGui::End();

	
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindVertexArray(VAOs[1]);
	
	//Render floor
	programs[2].use();
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[0], 0, sizeof(PVMposUBO));
	glDrawArrays(GL_TRIANGLES, sizeof(cubeVertices) / sizeof(float) / 5, sizeof(planeVertices) / (sizeof(float) * 5));
	
	//Render cube
	programs[0].use();
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[4], 0, sizeof(PVMUBO));
	glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeVertices) / (sizeof(float) * 5));

	//Render nanosuit
	//glBindVertexArray(VAOs[2]);
	//programs[6].use();
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[3]);
	//Update time
	glBindBuffer(GL_UNIFORM_BUFFER, UBOs[2]);
	float glfwTime = static_cast<float>(glfwGetTime());
	glBufferSubData(GL_UNIFORM_BUFFER, offsetof(RefractUBO, time), sizeof(float), &glfwTime);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[2], 0, sizeof(RefractUBO));
	models[0].draw(programs[6], true);
	//glDrawArrays(GL_TRIANGLES, 0, sizeof(cubeRefractVertices) / sizeof(float) / 6);

	//Render planet
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[5], 0, sizeof(PVMUBO));
	models[1].draw(programs[7], false);

	//Render asteroids
	//Asteroids
	programs[8].use();
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[3], 0, sizeof(PVMrotUBO));
	glBindBuffer(GL_UNIFORM_BUFFER, UBOs[3]);
	glm::mat4 M = glm::translate(glm::vec3(0.0f, 20.0f, -10.0f));
	M = glm::rotate(M, static_cast<float>(glfwGetTime() / 10.0f), glm::vec3(0.0f, 1.0, 0.0f));
	asteroidUBO.PVM = PV * M;
	glBufferSubData(GL_UNIFORM_BUFFER, offsetof(PVMrotUBO, PVM), sizeof(glm::mat4), &asteroidUBO.PVM);
	for (int i = 0; i < models[2].meshes.size(); i++)
	{
		asteroidUBO.rot = glm::rotate(static_cast<float>(glfwGetTime()), randAsteroidRotationVec[i]);
		glBufferSubData(GL_UNIFORM_BUFFER, offsetof(PVMrotUBO, rot), sizeof(glm::mat4), &asteroidUBO.rot);
		glBindVertexArray(models[2].meshes[i].VAO);
		glBindTexture(GL_TEXTURE_2D, models[2].texturesLoaded[0].id);
		glDrawElementsInstanced(GL_TRIANGLES, models[2].meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
	}
	
	//Render skybox
	glBindVertexArray(VAOs[0]);
	glDepthFunc(GL_LEQUAL);
	programs[3].use();
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[1], 0, sizeof(PVUBO));
	glDrawArrays(GL_TRIANGLES, 0, sizeof(skyboxVertices) / (sizeof(float) * 3));
	glDepthFunc(GL_LESS);
	
	glBindVertexArray(VAOs[1]);
	//Render each window
	programs[0].use();
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	for (unsigned int i = 0; i < windows.size(); i++)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOs[i + UBOs.size() - windows.size()], 0, sizeof(PVMUBO));
		glDrawArrays(GL_TRIANGLES, (sizeof(cubeVertices) + sizeof(planeVertices)) / (sizeof(float) * 5), sizeof(transparentVertices) / (sizeof(float) * 5));
	}

	//Colored houses 
	glBindVertexArray(VAOs[3]);
	programs[5].use();
	glDrawArrays(GL_POINTS, 0, sizeof(points) / (sizeof(float) * 5));
	
	//Blit to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	programs[1].use();
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

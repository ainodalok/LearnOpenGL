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

	glGenBuffers(1, &UBOlight);
	glBindBuffer(GL_UNIFORM_BUFFER, UBOlight);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLight) * POINT_LIGHTS_NUMBER + sizeof(DirSpotLight), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOlight, 0, sizeof(PointLight) * POINT_LIGHTS_NUMBER + sizeof(DirSpotLight));
	dirSpotLight =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::cos(glm::radians(12.5f)),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::cos(glm::radians(15.0f)),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		glm::vec3(0.05f, 0.05f, 0.05f),
		0.07f,
		glm::vec3(0.4f, 0.4f, 0.4f),
		0.017f,
		glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f)
	};
	for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
	{
		pointLights.push_back
		({
			pointLightPositions[i],
			glm::vec3(0.05f, 0.05f, 0.05f),
			1.0f,
			glm::vec3(0.8f, 0.8f, 0.8f),
			0.09f,
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.032f
		});
	}

	glBufferSubData(GL_UNIFORM_BUFFER, 0, pointLights.size() * sizeof(pointLights[0]), pointLights.data());
	glBufferSubData(GL_UNIFORM_BUFFER, pointLights.size() * sizeof(pointLights[0]), sizeof(dirSpotLight), &dirSpotLight);

	//Object program
	programs.push_back(0);
	programs[0] = new Shader("shaders/object.vert", "null", "shaders/object.frag");
	programs[0]->use();
	//Material
	glUniform1i(3, 0);	//Diffuse texture
	glUniform1i(4, 1);	//Specular texture
	glUniform1f(5, 32.0f);	//Specular object shininess
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
	float aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);
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

void Renderer::render(Camera& camera, bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Object
	programs[0]->use();
	glBindVertexArray(VAOs[0]);

	if (camera.wasUpdatedV)
	{
		//Light UBO
		glBindBuffer(GL_UNIFORM_BUFFER, UBOlight);
		dirSpotLight.directionDir = glm::vec4(glm::mat3(camera.getV()) * dirLightDirection, 0.0f);
		for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
		{
			pointLights[i].position = camera.getV() * pointLightPositions[i];
			glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(PointLight), sizeof(pointLights[i].position), glm::value_ptr(pointLights[i].position));
		}
		glBufferSubData(GL_UNIFORM_BUFFER, pointLights.size() * sizeof(PointLight) + 80, sizeof(dirSpotLight.directionDir), glm::value_ptr(dirSpotLight.directionDir));
	}

	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 objectM = glm::translate(cubePositions[i]);
		objectM = glm::rotate(objectM, glm::radians(static_cast<float>(glfwGetTime()) * 50.0f + 20 * i), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 objectVM = camera.getV() * objectM;

		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera.getP() * objectVM));	//PVM
		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(objectVM));	//VM
		glUniformMatrix3fv(2, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::adjugate(glm::mat3(objectVM)))));	//transposedAdjugateVM
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	//Light, not flashlight
	programs[1]->use();
	glBindVertexArray(VAOs[1]);
	for (int i = 0; i < POINT_LIGHTS_NUMBER; i++)
	{
		glm::mat4 lightM = glm::translate(glm::vec3(pointLightPositions[i]));
		lightM = glm::scale(lightM, glm::vec3(0.2f));
		glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera.getP() * camera.getV() * lightM));	//PVM
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	camera.wasUpdatedP = false;
	camera.wasUpdatedV = false;

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

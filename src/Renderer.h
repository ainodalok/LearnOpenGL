#ifndef RENDERER_H
#define RENDERER_H

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "UI.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <vector>
#include <random>
#include <chrono>

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();

	void render(Camera &camera, bool wireframe);
    void rebuildScreenFramebuffers(int width, int height);

private:

	typedef struct MVUBO
	{
		glm::mat4 VM;
		glm::mat4 cofactorVM;
	} MVUBO;

	typedef struct PUBO
	{
		glm::mat4 P;
	} PUBO;

	typedef struct LightUBO
	{
		glm::vec4 lightPos[32];
		glm::vec4 lightCol[32];
	} LightUBO;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	GLuint depthMapFBO = 0;
	GLuint depthMapTexture = 0;

	GLuint GBuffer = 0;
	GLuint textureGBuffer[3] = { 0, 0, 0 }; // position, normal, albedo + specular
	GLuint RBOGBuffer = 0;
	GLuint FBOScreen = 0;
	GLuint textureScreen[2] = { 0, 0 }; //0 - General resolution, 1 - Only bright areas
	GLuint RBOScreen = 0;
	GLuint FBOBloom = 0;
	GLuint textureBloom[2] = { 0, 0 };
	int bloomDownscaleFactor = 3;
	int bloomAmount = 4;
	
    int width = 0;
    int height = 0;

	bool bloom = true;

	std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
	std::vector<GLuint> textures;
	std::vector<Shader> programs;
    std::vector<GLuint> UBOs;
	std::vector<Model> models;

	MVUBO modelUBO;
	MVUBO lampUBO;
	PUBO cameraUBO;
	LightUBO lightUBO;

	glm::vec4 lightPosStart[32];
	glm::vec4 lightColStart[32];

	std::mt19937_64 rng;

	void load2DTexture(const std::string &texturePath, GLint wrapMode, bool srgb);
    void loadCubeMap(const std::vector<std::string> &texturePath);
	void buildDepthMapFramebuffer(GLuint &FBO, GLuint &texture);
	void renderScene();
	void renderForward();
	void updateUniforms(Camera& camera);
};

#endif
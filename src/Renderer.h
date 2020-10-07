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
#include <array>
#include <map>

class Renderer
{
public:
	Renderer(int width, int height);
	~Renderer();

	void render(Camera &camera, bool wireframe);
    void rebuildFramebuffer(int width, int height);

private:

	typedef struct MUBO
	{
		glm::mat4 M;
		glm::mat4 cofactorM;
	} MUBO;

	typedef struct PVUBO
	{
		glm::mat4 PV;
	} PV;

	typedef struct LightUBO
	{
		glm::vec4 viewPos;
		glm::vec3 lightPos;
	} LightUBO;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	GLuint depthMapFBO = 0;
	GLuint depthMapTexture = 0;
	
    GLuint FBO = 0;
    GLuint FBOtexture = 0;
    int width = 0;
    int height = 0;
    GLuint RBO = 0;

	std::vector<GLuint> VAOs;
    std::vector<GLuint> VBOs;
	std::vector<GLuint> textures;
	std::vector<Shader> programs;
    std::vector<GLuint> UBOs;

	MUBO planeUBO;
	PVUBO cameraUBO;
	LightUBO lightUBO;

	void load2DTexture(const std::string &texturePath, GLint wrapMode, bool srgb);
    void loadCubeMap(const std::vector<std::string> &texturePath);
	void buildDepthMapFramebuffer(GLuint &FBO, GLuint &texture);
	void renderScene();
	void updateUniforms(Camera& camera);
};

#endif

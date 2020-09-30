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
    typedef struct PVUBO
	{
		glm::mat4 PV;
	}PVUBO;

	typedef struct ObjectBlock
	{
		glm::mat4 M;
		glm::mat4 cofactorM; //transpose(adjugate)
	}ObjectBlock;
	
	typedef struct LightUBO
	{
		glm::vec4 lightPos;
		glm::vec4 viewPos;
	}LightUBO;

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

	PVUBO cameraUBO;
	ObjectBlock floorUBO;
	ObjectBlock cubeUBOs[3];
	LightUBO lightUBO;
	PVUBO directShadowUBO;

	void load2DTexture(const std::string &texturePath, GLint wrapMode, bool srgb);
    void loadCubeMap(const std::vector<std::string> &texturePath);
	void buildDepthMapFramebuffer(GLuint &FBO, GLuint &texture);
	void renderScene(bool shadow);
	void updateUniforms(Camera& camera);
};

#endif

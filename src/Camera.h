#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera(GLFWwindow* window);

	void updateP(int width, int height);
	void updateV();

	const glm::f32* getV() const;
	const glm::f32* getP() const;

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float speed = 0.005f;

private:
	glm::mat4 V;
	glm::mat4 P;

};

#endif


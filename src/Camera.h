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

	const glm::f32* getV() const;
	const glm::f32* getP() const;

private:
	glm::mat4 V;
	glm::mat4 P;

};

#endif


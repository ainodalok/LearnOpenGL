#include "Camera.h"

Camera::Camera(GLFWwindow* window)
{
	V = glm::mat4(1.0f);
	V = glm::translate(V, glm::vec3(0.0f, 0.0f, -3.0f));
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = (float)width / (float)height;
	//P = glm::perspective(glm::atan(glm::tan(glm::radians(90.0f) / 2.0f) * aspect) * 2.0f, aspect, 0.1f, 100.0f);
	P = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void Camera::updateP(int width, int height)
{
	float aspect = (float)width / (float)height;
	//P = glm::perspective(glm::atan(glm::tan(glm::radians(90.0f) / 2.0f) * aspect) * 2.0f, aspect, 0.1f, 100.0f);
	P = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

const glm::f32* Camera::getV() const
{
	return glm::value_ptr(V);
}

const glm::f32* Camera::getP() const
{
	return glm::value_ptr(P);;
}

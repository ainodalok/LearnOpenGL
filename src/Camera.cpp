#include "Camera.h"

Camera::Camera(GLFWwindow* window)
{
	updateV();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	updateP(width, height);
}

void Camera::updateP(int width, int height)
{
	float aspect = (float)width / (float)height;
	P = glm::perspective(glm::atan(glm::tan(glm::radians(90.0f) / 2.0f) / aspect) * 2.0f, aspect, 0.1f, 100.0f);
}

void Camera::updateV()
{
	V = glm::translate(glm::mat4(1), pos);
	V = V * glm::mat4_cast(orientation);
	V = glm::inverse(V);
}

const glm::f32* Camera::getV() const
{
	return glm::value_ptr(V);
}

const glm::f32* Camera::getP() const
{
	return glm::value_ptr(P);;
}

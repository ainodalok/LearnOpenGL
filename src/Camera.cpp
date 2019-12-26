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
	V = glm::translate(pos);
	V = V * glm::mat4_cast(orientation);
	V = glm::inverse(V);
}

const glm::mat4& Camera::getV() const
{
	return V;
}

const glm::mat4& Camera::getP() const
{
	return P;
}

float Camera::getYaw()
{
	//Avoid singularity with w = 0.0
	glm::quat noPitch = glm::rotate(orientation, -getPitch(), glm::normalize(orientation * initialRight));
	if (noPitch.w == 0)
		return glm::half_pi<float>();
	return glm::atan(noPitch.y / noPitch.w);
}

float Camera::getPitch()
{
	return glm::half_pi<float>() - glm::acos(glm::dot(initialUp, glm::normalize(orientation * initialFront)));
}

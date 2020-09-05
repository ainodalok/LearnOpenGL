#include "Camera.h"

Camera::Camera(GLFWwindow* window)
{
	needUpdateV = true;
	updateV();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	updateP(width, height);
}

void Camera::updateP(int width, int height)
{
	float aspect = (float)width / (float)height;
	P = glm::perspective(glm::atan(glm::tan(glm::radians(90.0f) / 2.0f) / aspect) * 2.0f, aspect, 0.1f, 100.0f);
	wasUpdatedP = true;
}

void Camera::updateV()
{
	if (needUpdateV)
	{
		V = glm::mat4_cast(glm::inverse(orientation));
		V = glm::translate(V, -pos);
		needUpdateV = false;
		wasUpdatedV = true;
	}
}

const glm::mat4& Camera::getV() const
{
	return V;
}

const glm::mat4& Camera::getP() const
{
	return P;
}
#include "UI.h"
float Camera::getYaw()
{
	//Avoid singularity with w = 0.0
	const glm::quat noPitch = orientation * glm::angleAxis(-getPitch(), initialRight);

	//Avoid singularity with w = 0.0
	if (orientation.w == 0)
		return glm::half_pi<float>();
	return 2 * glm::atan(noPitch.y / noPitch.w);
}

float Camera::getPitch()
{
	return glm::half_pi<float>() - glm::acos(glm::dot(initialUp, glm::normalize(orientation * initialFront)));
}

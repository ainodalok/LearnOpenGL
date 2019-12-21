#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	const glm::vec3 initialFront = glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3 initialUp = glm::vec3(0.0f, 1.0f, 0.0f);

	Camera(GLFWwindow* window);

	void updateP(int width, int height);
	void updateV();

	const glm::f32* getV() const;
	const glm::f32* getP() const;

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::quat orientation = glm::quatLookAt(initialFront, initialUp);
	float speed = 0.005f;

private:
	glm::mat4 V;
	glm::mat4 P;

};

#endif


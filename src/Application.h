#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stdlib.h> 
#include <iostream>
#include <chrono>
#include <limits>

#include "UI.h"
#include "Renderer.h"
#include "Camera.h"
#include "Input.h"

class Application
{
public:
	Application();
	~Application();

	void executeLoop();

private:
	const ImGuiWindowFlags overlayBox =	ImGuiWindowFlags_NoDecoration |
										ImGuiWindowFlags_AlwaysAutoResize |
										ImGuiWindowFlags_NoFocusOnAppearing |
										ImGuiWindowFlags_NoNav;

	GLFWwindow* window;
	UI* ui;
	Renderer* renderer;
	Camera* camera;
	Input* input;

	bool focus = false;
	float previousMouseX = 0.0f;
	float previousMouseY = 0.0f;
	float sensitivity = 0.1f;
	bool firstCallbackInFocus = true;

	unsigned long long int numberOfFrames = 0;
	double timeSinceStart = 0.0;
	unsigned long long int avgNumberOfFrames = 0;
	double avgTimeSinceStart = 0.0;
	std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();

	void draw();
	void handleInput();
	void ImGuiPerformanceBox();

	static void glfwErrorCallback(int error, const char* description);
	static void glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
	static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void glfwMouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

#endif
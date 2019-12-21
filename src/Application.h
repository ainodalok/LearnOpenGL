#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h> 
#include <iostream>
#include <chrono>

#include "UI.h"
#include "Mesh.h"
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
	Mesh* mesh;
	Camera* camera;
	Input* input;

	void draw();
	void handleInput();
	void ImGuiPerformanceBox(std::chrono::high_resolution_clock::time_point& previousTime);

	static void glfwErrorCallback(int error, const char* description);
	static void glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
	static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
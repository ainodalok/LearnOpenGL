#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h> 
#include <iostream>

#include "UI.h"
#include "Mesh.h"
#include "Camera.h"

class Application
{
public:
	Application();
	~Application();

	void executeLoop();

private:
	GLFWwindow* window;
	UI* ui;
	Mesh* mesh;
	Camera* camera;

	void draw();

	static void glfwErrorCallback(int error, const char* description);
	static void glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
	static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif
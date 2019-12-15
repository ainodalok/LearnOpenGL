#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h> 
#include <iostream>

#include "UI.h"
#include "Mesh.h"

class Application
{
public:
	Application();
	~Application();

	void executeLoop();

private:
	GLFWwindow* window;
	UI* ui;
	Mesh* quadrangle;

	void draw();

	static void glfwErrorCallback(int error, const char* description);
};

#endif
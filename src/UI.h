#ifndef UI_H
#define UI_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const ImGuiWindowFlags overlayBox = ImGuiWindowFlags_NoDecoration |
	ImGuiWindowFlags_AlwaysAutoResize |
	ImGuiWindowFlags_NoFocusOnAppearing |
	ImGuiWindowFlags_NoNav;

class UI
{
public:
	UI(GLFWwindow* window);
	~UI();

private:
	ImGuiIO* io;
};

#endif
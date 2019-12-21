#include "Application.h"

#define DEBUG 0

void Application::glfwErrorCallback(int error, const char* description)
{
	std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

void Application::glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (width > 0 && height > 0)
	{
		app->camera->updateP(width, height);
		app->draw();
	}
}

void Application::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (action != GLFW_REPEAT)
		if (app->input->keyData.find(key) != app->input->keyData.end())
			app->input->keyEvents.push({ key, action, std::chrono::high_resolution_clock::now() });
}

void Application::glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
	}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
	}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
	}();

	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << std::endl;
}

Application::Application()
{
	if (!glfwInit())
		std::cerr << "Failed to initialize GLFW" << std::endl;
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	window = glfwCreateWindow(1280, 720, "Application", /*glfwGetPrimaryMonitor()*/NULL, NULL);
	if (!window)
		std::cerr << "Failed to create window" << std::endl;
	glfwSetWindowSizeLimits(window, 640, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cerr << "Failed to load GL functions" << std::endl;
	glfwSetWindowUserPointer(window, this);
	glfwSetErrorCallback(glfwErrorCallback);
	glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);
#if DEBUG == 1
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glMessageCallback, nullptr);
#endif
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	input = new Input();
	ui = new UI(window);
	camera = new Camera(window);
	mesh = new Mesh();
}

Application::~Application()
{
	delete input;
	delete ui;
	delete camera;
	delete mesh;
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Application::executeLoop()
{
	std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		input->resolveKeyEvents();

		handleInput();
		input->frameFinished();

		ImGuiPerformanceBox(previousTime);

		ImGui::Render();

		draw();
	}
}

void Application::draw()
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	camera->updateV();
	mesh->render(*camera, false);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

void Application::handleInput()
{
	if (input->getKeySinglePressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("Movement durations/Frame", (bool*) 0, overlayBox);
		ImGui::Text("W %f ms", input->getKeyDuration(GLFW_KEY_W));
		ImGui::Text("S %f ms", input->getKeyDuration(GLFW_KEY_S));
		ImGui::Text("A %f ms", input->getKeyDuration(GLFW_KEY_A));
		ImGui::Text("D %f ms", input->getKeyDuration(GLFW_KEY_D));
	ImGui::End();
}

void Application::ImGuiPerformanceBox(std::chrono::high_resolution_clock::time_point &previousTime)
{
	std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
	double timePassed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(currentTime - previousTime).count();
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::Begin("FPS overlay", (bool*)0, overlayBox);
	ImGui::Text("%f ms/Frame", timePassed);
	ImGui::Text("%d FPS", static_cast<unsigned>(1000.0 / timePassed));
	ImGui::End();
	previousTime = currentTime;
}

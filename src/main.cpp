#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "Application.h"

int main()
{
	stbi_set_flip_vertically_on_load(true);

	//Initializes GLFW window
	Application app;

	app.executeLoop();
	
	return 0;
}
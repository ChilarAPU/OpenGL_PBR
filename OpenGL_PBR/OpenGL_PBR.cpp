#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

//Macros
#define VIEWPORTHEIGHT 600
#define VIEWPORTWIDTH 800

//Forward function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
	//initialize GLFW 
	glfwInit();

	//Congifure GLFW window to use OpenGL 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//Set OpenGL to core as opposed to compatibility mode
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window object at the size of 800 x 600
	GLFWwindow* window = glfwCreateWindow(VIEWPORTWIDTH, VIEWPORTHEIGHT, "OpenGL_PBR", NULL, NULL);
	//Null check the window in case a parameter has failed
	if (window == NULL) 
	{
		cout << " GLFW Failed to initialize" << endl;
		glfwTerminate();
		return -1;
	}

	/*Check that GLAD has loaded properly using glfwGetProcAddress to pass through 
	the correct path based on the current OS */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to load GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, VIEWPORTWIDTH, VIEWPORTHEIGHT);

	// Assign callback function for whenever the user adjusts the viewport size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return 0;
}

/* Called whenever the user resizes the window containing the viewport*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}
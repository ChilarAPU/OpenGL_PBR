#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

using namespace std;

//Macros
#define VIEWPORTHEIGHT 600
#define VIEWPORTWIDTH 800

//Forward function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
string readShaderFile(const char* fileName);

//Temporary vertices for a 2d Triangle and indices
float vertices[] = {
	0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};

unsigned int indices[] = { 
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

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

	//Set window to the current context in the thread
	glfwMakeContextCurrent(window);

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

	//Basic VAO 
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //Bind VAO to store any subsequent VBO & EBO calls

	//Basic EBO for indices
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Basic VBO Setup for vertices
	unsigned int VBO;
	glGenBuffers(1, &VBO); //Create a single buffer for the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind Buffer to GL_ARRAY_BUFFER type
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //allocate memory and copy vertices to buffer
	//How are the vertices stored
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind VAO, EBO and VBO. MUST UNBIND VAO FIRST
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Basic compilation of shaders
	unsigned int vertexShader;
	string temp = readShaderFile("vertexShader.vs");
	const char* vertexS = temp.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER); //Set vertexShader as an empty shader object
	glShaderSource(vertexShader, 1, &vertexS, NULL); //replace the contents of the shader object 
	glCompileShader(vertexShader); //Compile new shader contents
	//Check whether the compilation was a success
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) //If shader did not compile
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); //Get compiation log
		cout << "ERROR SHADER VERTEX COMPILATION FAILED\n" << infoLog << endl; //Output error log
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	temp = readShaderFile("fragmentShader.fs");
	const char* fragmentS = temp.c_str();
	glShaderSource(fragmentShader, 1, &fragmentS, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); //Get compiation log
		cout << "ERROR SHADER FRAGMENT COMPILATION FAILED\n" << infoLog << endl; //Output error log
	}

	//Create Shader Program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); //Set shaderProgram as empty program object
	//Attach shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); //Link program to shaders together
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR LINKING SHADERS\n" << infoLog << endl;
	}

	glUseProgram(shaderProgram); //tell subsequent render calls to use this shader program
	//Delete shader objects as they are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Run the window until explicitly told to stop
	while (!glfwWindowShouldClose(window))  //Check if the window has been instructed to close
	{
		processInput(window); //Process user inputs

		//Clear colour buffer every frame before rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Wireframe Mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//Basic Rendering
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window); //Render the coloured pixels to the window
		glfwPollEvents(); //Check if any user events have been triggered
	}
	
	//Clean up GLFW resources as we now want to close the program
	glfwTerminate();
	return 0;
}

string readShaderFile(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good()) //File failed to open
	{
		cout << "File Failed to Load" << fileName << endl;
		return {};
	}
	/*string s;
	while (meInput)
	{
		s += meInput.get();
	}
	meInput.close();
	return s;
	*/
	
	//iterate over the contents of the file
	return string(
		istreambuf_iterator<char>(meInput),
		istreambuf_iterator<char>());
	meInput.close();
	
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

/* Called whenever the user resizes the window containing the viewport*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}
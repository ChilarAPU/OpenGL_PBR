#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION //Effectively turns header file into a cpp file
#include <STB/stb_image.h>

#include "Shader.h"
#include "Camera.h"

using namespace std;
using namespace glm;

//Macros
#define VIEWPORTHEIGHT 600
#define VIEWPORTWIDTH 800

//Forward function declarations
/* Called Whenever user changes the size of the viewport*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
/* Handles player Input*/
void processInput(GLFWwindow* window);
/* Initialize OpenGL window as well as GLFW and glad libraries*/
void initWindow(GLFWwindow*& window);
/* Render polygon to screen */
void display(Shader shaderToUse);

void mouseCallback(GLFWwindow* window, double xPosition, double yPosition);

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

/* Bind cube to passed through VAO*/
void bindCubeToVAO(unsigned int& vao);

string readShaderFile(const char* fileName); //Unused

//temporary place for object buffer data
float Cubevertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

//temporary place for buffer objects
unsigned int VAO;
unsigned int texture;
unsigned int texture2;
unsigned int lightVAO;

unsigned int fps;

//temp camera
vec3 cameraUp = vec3(0.0, 1.0, 0.0);

float deltaTime = 0.0f; //Time between current and last frame;
float lastFrame = 0.0f; //Time of last frame

Camera* camera = new Camera(vec3(0.0, 0.0, 3.0), 45.f);

Shader* lightShader;

int main() {

	//Initialize window and set it to main viewport
	GLFWwindow* window;
	initWindow(window);


	// Assign callback function for whenever the user adjusts the viewport size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Bind Main Object cube to VAO
	bindCubeToVAO(VAO);

	//Bind cube data to new buffer for light
	bindCubeToVAO(lightVAO);

	//tell stbi to flip incoming images on load
	stbi_set_flip_vertically_on_load(true);

	//Load texture from file using stb
	int width, height, nrChannels;
	unsigned char* data = stbi_load("../brickwall.jpg", &width, &height, &nrChannels, 0);

	//Generate texture into shader 
	glGenTextures(1, &texture); //Generate 1 texture buffer
	glBindTexture(GL_TEXTURE_2D, texture);

	//How should we handle texture coordinates outside of 0-1
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Texture filtering for both scalling up and down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); //Automatically generate mipmap for passed through texture
	}
	else 
	{
		cout << "FAILED TO LOAD TEXTURE 'brickwall.jpg'" << endl;
	}
	

	//Free memory that holds the source texture data
	stbi_image_free(data);

	data = stbi_load("../carlos.jpg", &width, &height, &nrChannels, 0);

	//Generate texture into shader 
	glGenTextures(1, &texture2); //Generate 1 texture buffer
	glBindTexture(GL_TEXTURE_2D, texture2);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); //Automatically generate mipmap for passed through texture
	}
	else
	{
		cout << "FAILED TO LOAD TEXTURE 'carlos.jpg'" << endl;
	}


	Shader currentShader("vertexShader.vert", "fragmentShader.frag");
	//Bind light shader
	lightShader = new Shader("lightShader.vert", "lightShader.frag");

	//Tell shader which sampler belongs to which texture unit
	currentShader.use();
	currentShader.setInt("ourTexture1", 0);
	currentShader.setInt("ourTexture2", 1);

	//temp FPS calc
	float time = 1.f; //Time to delay for

	float cachedTime = glfwGetTime(); //Holds time of previous frame

	//Hide cursor and capture it inside the window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Bind mouse function callback to mouseCallback()
	glfwSetCursorPosCallback(window, mouseCallback);

	//Scroll wheel callback
	glfwSetScrollCallback(window, scrollCallback);


	//Run the window until explicitly told to stop
	while (!glfwWindowShouldClose(window))  //Check if the window has been instructed to close
	{
		//calculate delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window); //Process user inputs

		glEnable(GL_DEPTH_TEST); //Tell OpenGL to use Z-Buffer

		//Clear colour buffer and depth buffer every frame before rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display(currentShader);

		//temp looping timer
		float now = glfwGetTime(); //Get time of current frame
		float delta = now - cachedTime; //difference between current and previous frame
		cachedTime = now; //Update cachedTime to hold new time

		fps++;
		time -= delta; //subtract difference from specified time
		if (time <= 0.f) //Have we used up the alloted time
		{
			cout << fps << " fps" << endl;
			fps = 0; //Reset FPS counter
			time = 1; //Reset timer
		}

		glfwSwapBuffers(window); //Swap to the front/back buffer once the buffer has finished rendering
		glfwPollEvents(); //Check if any user events have been triggered

	}
	
	//Clean up GLFW resources as we now want to close the program
	glfwTerminate();
	return 0;
}

void initWindow(GLFWwindow*& window)
{
	//initialize GLFW 
	glfwInit();

	//Congifure GLFW window to use OpenGL 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//Set OpenGL to core as opposed to compatibility mode
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window object at the size of 800 x 600
	window = glfwCreateWindow(VIEWPORTWIDTH, VIEWPORTHEIGHT, "OpenGL_PBR", NULL, NULL);
	//Null check the window in case a parameter has failed
	if (window == NULL)
	{
		cout << " GLFW Failed to initialize" << endl;
		glfwTerminate();
		return;
	}

	//Set window to the current context in the thread
	glfwMakeContextCurrent(window);

	/*Check that GLAD has loaded properly using glfwGetProcAddress to pass through
	the correct path based on the current OS */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to load GLAD" << endl;
		return;
	}

	glViewport(0, 0, VIEWPORTWIDTH, VIEWPORTHEIGHT);
}

void display(Shader shaderToUse)
{
	//Wireframe Mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//Adjust uniform color value over time
	float systemTime = glfwGetTime();
	float greenValue = (sin(systemTime) / 2.0f) + 0.5f;
	//int vertexColorLocation = glGetUniformLocation(currentShader, "newColor");
	//currentShader.setFloat("newColor", greenValue);

	mat4 view = mat4(1.0);
	mat4 projection = mat4(1.0);

	//model = rotate(model, (float)glfwGetTime() * radians(50.f), vec3(.5, 1.0, 0.0));
	projection = perspective(camera->GetFOV(), (float)VIEWPORTWIDTH / (float)VIEWPORTHEIGHT, 0.1f, 100.f);

	//Final parameter is just the up vector of the world in world space meaning it never changes
	view = camera->GetViewMatrix();

	//Basic Rendering
	shaderToUse.use();
	//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	glBindVertexArray(VAO);

	//pass through view and projection matrix to shader
	shaderToUse.setMat4("view", view);
	shaderToUse.setMat4("projection", projection);

	//Texture will be binded to GL_TEXTURE0
	glActiveTexture(GL_TEXTURE0);
	//Bind texture to fragment shader
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//Position of the light source
	vec3 lightPos(1.2f, 1.0f, 2.0f);
	//Move light source around a circle with a radius of 3 and a speed of 1
	lightPos.x = cos(1.0 * glfwGetTime()) * 3.f;
	lightPos.z = sin(1.0 * glfwGetTime()) * 3.f;

	//adjust light colour over time
	vec3 lightColor;
	lightColor.x = sin((glfwGetTime()) * 2.0f);
	lightColor.y = sin((glfwGetTime()) * 0.7f);
	lightColor.z = sin((glfwGetTime()) * 1.3f);

	vec3 diffuseColor = lightColor * vec3(0.5f);
	vec3 ambientColor = diffuseColor * vec3(0.2f);

	//Use basic colours as opposed to textures
	shaderToUse.setVec3("objectColor", vec3(1.0, 0.5, 0.31));
	shaderToUse.setVec3("lightColor", vec3(1.0, 1.0, 1.0));
	shaderToUse.setVec3("lightPos", lightPos);
	shaderToUse.setVec3("viewPos", camera->GetPosition());

	//Set Material struct uniforms
	shaderToUse.setVec3("material.ambient", vec3(1.0f, 0.5f, 0.31f));
	shaderToUse.setVec3("material.diffuse", vec3(1.0f, 0.5f, 0.31f));
	shaderToUse.setVec3("material.specular", vec3(0.5f, 0.5f, 0.5f));
	shaderToUse.setFloat("material.shininess", 32.0f);
	
	//Set Light struct uniforms
	//We do this so that each lighting technique has different strength amounts
	shaderToUse.setVec3("light.ambient", ambientColor);
	shaderToUse.setVec3("light.diffuse", diffuseColor);
	shaderToUse.setVec3("light.specular", vec3(1.0f, 1.0f, 1.0f));
	shaderToUse.setVec3("light.position", lightPos);

	//Draw multiple of the same object with varying translation vectors in world space
	for (unsigned int i = 0; i < 10; i++)
	{
		mat4 model = mat4(1.0);
		model = translate(model, cubePositions[i]); //set world position location of object
		float angle = 20.f * i;
		model = rotate(model, (float)glfwGetTime() * radians(angle), vec3(1.0f, 0.3f, 0.5f)); //Rotate object in world space
		shaderToUse.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Use different shader for the source of the light
	lightShader->use();
	lightShader->setMat4("projection", projection);
	lightShader->setMat4("view", view);
	//Scale light and move it in world space
	mat4 model(1.0f);
	model = translate(model, lightPos);
	model = scale(model, vec3(0.2f));
	lightShader->setMat4("model", model);
	//Set colour of the light object
	lightShader->setVec3("lightColor", lightColor);
	//call VAO that holds the buffer and draw it to the screen
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void bindCubeToVAO(unsigned int& vao)
{
	//Basic VAO 
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); //Bind VAO to store any subsequent VBO & EBO calls

	//Basic EBO for indices
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/

	//Basic VBO Setup for vertices
	unsigned int VBO;
	glGenBuffers(1, &VBO); //Create a single buffer for the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind Buffer to GL_ARRAY_BUFFER type
	/*glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //allocate memory and copy vertices to buffer
	//How are the vertices stored
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cubevertices), Cubevertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//storing color attribute into buffer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//storing texture coordinates into buffer
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(3);

	//Unbind VAO, EBO and VBO. MUST UNBIND VAO FIRST
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera->AdjustFOV((float)yOffset, 45.0f);
}

void mouseCallback(GLFWwindow* window, double xPosition, double yPosition)
{
	camera->CalculateMouseAdjustment(xPosition, yPosition);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	//call KeyboardMovement for basic movement on the camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->KeyboardMovement(EMovementDirection::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->KeyboardMovement(EMovementDirection::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->KeyboardMovement(EMovementDirection::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->KeyboardMovement(EMovementDirection::RIGHT, deltaTime);
	}
}

/* Called whenever the user resizes the window containing the viewport*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}
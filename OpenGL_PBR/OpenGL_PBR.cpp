#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION //Effectively turns header file into a cpp file
#include <STB/stb_image.h>
#include <assimp/config.h>
#include <map>

#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"

using namespace std;
using namespace glm;

//Macros
#define VIEWPORTHEIGHT 720
#define VIEWPORTWIDTH 1280	

//Forward function declarations
/* Called Whenever user changes the size of the viewport*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
/* Handles player Input*/
void processInput(GLFWwindow* window);
/* Initialize OpenGL window as well as GLFW and glad libraries*/
void initWindow(GLFWwindow*& window);
/* Render polygon to screen */
void display(Shader shaderToUse, Model m);

void mouseCallback(GLFWwindow* window, double xPosition, double yPosition);

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

/* Bind cube to passed through VAO*/
void bindCubeToVAO(unsigned int& vao);

//temporary place for object buffer data
float Cubevertices[] = {
	// Back face
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
	   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	   // Front face
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	   // Left face
	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	   // Right face
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left       
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

vec3 pointLightPositions[] = {
	vec3(0.7f,  0.2f,  2.0f),
	vec3(2.3f, -3.3f, -4.0f),
	vec3(-4.0f,  2.0f, -12.0f),
	vec3(0.0f,  0.0f, -3.0f)
};

vec3 pointLightColorss[] = {
	vec3(1.0f,  0.0f,  0.0f),
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f,  0.0f, 1.0f),
	vec3(1.0f,  0.0f, 1.0f)
};

float transparentVertices[] = {
	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	/// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

vector<vec3> vegetation;

//temporary place for buffer objects
unsigned int VAO;
unsigned int lightVAO;
unsigned int vegetationVAO;
unsigned int screenQuadVAO;
unsigned int skyboxVAO;
unsigned int textureColorbuffer;
unsigned int rbo;
unique_ptr<Texture> cubeDiffuse(new Texture());
unique_ptr<Texture> cubeSpecular(new Texture());
unique_ptr<Texture> cubeEmissive(new Texture());
unique_ptr<Texture> grassTexture(new Texture());

unsigned int cubemapTexture;

Model* windowModel = new Model();

unsigned int fps;
float frameTime = 0;

//temp camera
vec3 cameraUp = vec3(0.0, 1.0, 0.0);

float deltaTime = 0.0f; //Time between current and last frame;
float lastFrame = 0.0f; //Time of last frame

Camera* camera = new Camera(vec3(0.0, 0.0, 3.0), 45.f);

Shader* lightShader;

Shader* skyboxShader;

map<float, vec3> sortedWindows; //Holds a sorted map of window positions so that they can be drawn in the correct order

unsigned int framebuffer; //custom framebuffer delcaration

Shader* screenSpaceShader;

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

	//Load and bind textures to buffers
	/*cubeDiffuse->LoadTexture("../textures/container2.png");
	cubeSpecular->LoadTexture("../textures/container2_specular.png");
	cubeEmissive->LoadTexture("../textures/matrix.jpg");
	cubeDiffuse->BindTextureToBuffer(GL_TEXTURE0);
	cubeSpecular->BindTextureToBuffer(GL_TEXTURE1);
	cubeEmissive->BindTextureToBuffer(GL_TEXTURE2);
	*/

	Shader currentShader("vertexShader.vert", "fragmentShader.frag");
	//Bind light shader
	lightShader = new Shader("lightShader.vert", "lightShader.frag");

	screenSpaceShader = new Shader("screenSpaceShader.vert", "screenSpaceShader.frag");

	//temp FPS calc
	float time = 1.f; //Time to delay for

	float cachedTime = glfwGetTime(); //Holds time of previous frame

	//Hide cursor and capture it inside the window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Bind mouse function callback to mouseCallback()
	glfwSetCursorPosCallback(window, mouseCallback);

	//Scroll wheel callback
	glfwSetScrollCallback(window, scrollCallback);

	//instanciate model class
	Model* backpack = new Model();
	//load model textures
	backpack->setDiffuseDirectory("../textures/swordTextures/Albedo.png");
	backpack->setSpecularDirectory("../textures/swordTextures/Metallic.png");
	//load model into buffers
	backpack->loadModel("../textures/Katana_export.fbx");

	//load window model

	//temporary grass locations
	vegetation.push_back(vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(vec3(0.5f, 0.0f, -0.6f));

	glGenVertexArrays(1, &vegetationVAO);
	glBindVertexArray(vegetationVAO); //Bind VAO to store any subsequent VBO & EBO calls

	unsigned int VBO;
	glGenBuffers(1, &VBO); //Create a single buffer for the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind Buffer to GL_ARRAY_BUFFER type

	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//storing texture coordinates into buffer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	grassTexture->LoadTexture("../textures/blending_transparent_window.png");

	//Sort locations into a sorted order form the location of the camera
	//NEED TO CHANGE:: CURRENTLY IS ONLY SET ONCE AT THE BEGINNING OF THE RENDER AND NEVER UPDATED. NEEDS MORE CHECKING AND PERFORMANCE
	//CONSIDERATIONS BEFORE PUTTING INSIDE THE RENDER LOOP AS MAP BECOMES A MEMORY HOG
	for (int i = 0; i < vegetation.size(); i++)
	{
		float distance = length(camera->GetPosition() - vegetation[i]); //get distance from camera to window position
		sortedWindows[distance] = vegetation[i];
	}

	//Generate VAO for screen space quad
	unsigned int quadVBO;
	glGenVertexArrays(1, &screenQuadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(screenQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	screenSpaceShader->setInt("screenTexture", 0);

	//Create custom framebuffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//Create attachment for framebuffer. Basically memory location buffer such as VAO

	//texture attachment 
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

	//allocate memory for texture but do not fill it. Also set texture to size of viewport
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, VIEWPORTWIDTH, VIEWPORTHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach color attachment to framebuffer from texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//glReadBuffer(GL_COLOR_ATTACHMENT0);

	//glBindTexture(GL_TEXTURE_2D, 0);

	//renderbuffer object attachment
	//unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	//create depth and stencil renderbuffer object
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, VIEWPORTWIDTH, VIEWPORTHEIGHT);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//Check that the current frambuffer checks the minimum requirements 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << glCheckFramebufferStatus(GL_FRAMEBUFFER) << endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//End of framebuffer

	//Create vector of cubemap locations in the order that OpenGL uses
	vector<string> textures_faces;
	textures_faces.push_back("../textures/anime-skybox/right.png");
	textures_faces.push_back("../textures/anime-skybox/left.png");
	textures_faces.push_back("../textures/anime-skybox/top.png");
	textures_faces.push_back("../textures/anime-skybox/bottom.png");
	textures_faces.push_back("../textures/anime-skybox/front.png");
	textures_faces.push_back("../textures/anime-skybox/back.png");

	//Create cubemap texture
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	//Assign each texture location to its corresponding texture target
	int width, height, nrChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < textures_faces.size(); i++)
	{
		data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << textures_faces[i] << std::endl;
			stbi_image_free(data);
		}
	} 

	//assign cubemap texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//Bind VAO to skybox vertices
	unsigned int skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	//Only using vertices for the skybox
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Unbind VAO and VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	skyboxShader = new Shader("skybox.vert", "skybox.frag");
	skyboxShader->use();
	skyboxShader->setInt("skybox", 0);


	//Run the window until explicitly told to stop
	while (!glfwWindowShouldClose(window))  //Check if the window has been instructed to close
	{
		//calculate delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window); //Process user inputs
		//draw scene into offscreen frame buffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		//glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);

		//Clear colour buffer and depth buffer every frame before rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST); //Tell OpenGL to use Z-Buffer
		glDepthFunc(GL_LEQUAL); //Needed for skybox otherwise it has z-conflict with normal background

		glEnable(GL_STENCIL_TEST); //Enable stencil testing to add outlines to lights

		glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE); //Decides what to do when a stencil buffer either passes or fails
		/* if the stencil test fails, do nothing. If the depth test fails, keep the stencil buffer object the same. This will
		* result in the outline staying as an outline when hidden behind other objects that are not in the buffer. If both the stencil
		* and depth test pass, then do the same as when the depth test fails except this time the original object will be in view.
		*/

		glEnable(GL_BLEND); //Allow for blending between colours with transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO); //Only allow blending to affect alpha values

		glEnable(GL_CULL_FACE); //enable face culling
		glCullFace(GL_BACK); //Cull back faces
		glFrontFace(GL_CW); //counter clockwise ordering of faces. This is the usual default bahaviour of exported models

		display(currentShader, *backpack);

		//go to default framebuffer and render texture into visible window
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		screenSpaceShader->use(); //use screen space shader
		glClear(GL_COLOR_BUFFER_BIT); //clear color bit of original buffer
		glDisable(GL_DEPTH_TEST); //there will be no depth to destroy in screen space
		glDisable(GL_CULL_FACE); //disable culling otherwise screenQuad will be automatically destroyed for being too close to camera
		//render the quad to which the texture will be displayed
		glBindVertexArray(screenQuadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		screenSpaceShader->setInt("screenTexture", 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		

		//temp looping timer
		float now = glfwGetTime(); //Get time of current frame
		float delta = now - cachedTime; //difference between current and previous frame
		cachedTime = now; //Update cachedTime to hold new time
		
		//calculate frametime
		if (delta > frameTime) //Set frametime to the highest value
		{
			frameTime = delta;
			frameTime *= 1000; //Multiply by 1000 so we get the frametime in a much easir to read format
		}

		fps++;
		time -= delta; //subtract difference from specified time
		if (time <= 0.f) //Have we used up the alloted time
		{
			cout << fps << " fps" << endl;
			cout << "Frametime: " << frameTime << "ms" << endl;
			fps = 0; //Reset FPS counter
			time = 1; //Reset timer
			frameTime = 0; //Reset framtime

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

void display(Shader shaderToUse, Model m)
{
	//Wireframe Mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Do not write to the stencil buffer for undesired objects (loaded models)
	glStencilMask(0x00); //0x00 just means that we cannot update the stencil buffer

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
	shaderToUse.setBool("bIsTransparent", false);
	//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	glBindVertexArray(VAO);

	//pass through view and projection matrix to shader
	shaderToUse.setMat4("view", view);
	shaderToUse.setMat4("projection", projection);

	//Position of the light source
	vec3 lightPos(1.2f, 1.0f, 2.0f);
	//Move light source around a circle with a radius of 3 and a speed of 1
	//lightPos.x = cos(1.0 * glfwGetTime()) * 3.f;
	//lightPos.z = sin(1.0 * glfwGetTime()) * 3.f;

	//adjust light colour over time
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	//lightColor.x = sin((glfwGetTime()) * 2.0f);
	//lightColor.y = sin((glfwGetTime()) * 0.7f);
	//lightColor.z = sin((glfwGetTime()) * 1.3f);

	vec3 diffuseColor = lightColor * vec3(0.8f);
	vec3 ambientColor = diffuseColor * vec3(0.7f);

	shaderToUse.setVec3("viewPos", camera->GetPosition());

	//Set Material struct uniforms
	shaderToUse.setFloat("material.shininess", 64.0f);
	//Assign relevant texture unit to sampler
	//shaderToUse.setInt("material.diffuse", 0);
	shaderToUse.setInt("material.specular", 1);
	shaderToUse.setInt("material.emissive", 2);

	//Directional Light
	shaderToUse.setVec3("dirLight.direction", vec3(0.7f, 0.2f, 2.0f));
	shaderToUse.setVec3("dirLight.ambient", ambientColor);
	shaderToUse.setVec3("dirLight.diffuse", diffuseColor);
	shaderToUse.setVec3("dirLight.specular", lightColor);

	//Pass through light values for point lights
	for (int i = 0; i < 4; i++)
	{
		//First part of struct uniform 
		string t = "pointLights[";
		t += to_string(i);

		shaderToUse.setVec3(t + "].position", pointLightPositions[i]);
		shaderToUse.setVec3(t + "].diffuse", pointLightColorss[i] * vec3(0.5));
		shaderToUse.setVec3(t + "].ambient", pointLightColorss[i] * vec3(0.1));
		shaderToUse.setVec3(t + "].specular", pointLightColorss[i]);
		shaderToUse.setFloat(t + "].constant", 1.0f);
		shaderToUse.setFloat(t + "].linear", 0.09f);
		shaderToUse.setFloat(t + "].quadratic", 0.032f);
	}

	shaderToUse.setVec3("spotLight.position", camera->GetPosition());
	shaderToUse.setVec3("spotLight.direction", camera->GetForwardVector());
	shaderToUse.setFloat("spotLight.cutOff", cos(radians(12.5f)));
	shaderToUse.setFloat("spotLight.outerCutOff", cos(radians(17.5f)));
	shaderToUse.setVec3("spotLight.ambient", vec3(1.0f));
	shaderToUse.setVec3("spotLight.diffuse", vec3(1.0f));
	shaderToUse.setVec3("spotLight.specular", vec3(1.0f));

	//Draw sword
	mat4 model = mat4(1.0);
	model = scale(model, vec3(3.0, 3.0, 3.0));
	model = rotate(model, (float)radians(90.f), vec3(1.0f, 0.0, 0.0));
	shaderToUse.setMat4("model", model);
	m.Draw(shaderToUse, 0);
	//Draw sheathe
	model = translate(model, vec3(0.2, 0.0, 0.0));
	shaderToUse.setMat4("model", model);
	m.Draw(shaderToUse, 1);

	//Use different shader for the source of the light
	lightShader->use();
	lightShader->setMat4("projection", projection);
	lightShader->setMat4("view", view);
	//Draw 4 cube lights at different positions with different colours
	for (unsigned int i = 0; i < 4; i++)
	{
		//Draw objects as normal but writing them to the stencil buffer
		
		//Add cubes to stencil buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF); //All stencils will pass the stencil test
		glStencilMask(0xFF); //enable writing to the stencil buffer

		//Scale light and move it in world space
		mat4 model(1.0f);
		model = translate(model, pointLightPositions[i]);
		model = scale(model, vec3(0.2f));
		lightShader->setMat4("model", model);
		//Set colour of the light object
		lightShader->setVec3("lightColor", pointLightColorss[i]);
		//call VAO that holds the buffer and draw it to the screen
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Draw scaled cube around object to act as an outline but not writing these to the stencil buffer
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //Pass if depth value is not equal to the stored depth
		glStencilMask(0x00); //disable writing to the stencil buffer
		glDisable(GL_DEPTH_TEST);
		// Set colour of outline
		lightShader->setVec3("lightColor", pointLightColorss[i] * vec3(0.5f)); //Get outline tint based on original colour
		//Increase size of the cube
		model = scale(model, vec3(1.2f));
		lightShader->setMat4("model", model);
		lightShader->use();
		//Draw cube
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
	}

	//load multiple transparent grass
	shaderToUse.use();
	glBindVertexArray(vegetationVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTexture->GetID());
	shaderToUse.setInt("grassTexture", 0);
	shaderToUse.setBool("bIsTransparent", true);
	//Loop through window position in the reverse order so that the furthest away windows are always drawn first
	for (map<float, vec3>::reverse_iterator it = sortedWindows.rbegin(); it != sortedWindows.rend(); ++it)
	{
		model = mat4(1.0f);
		model = translate(model, it->second);
		shaderToUse.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//This fixes the problem of windows not accounting for other windows that are behind them
	}
	shaderToUse.setBool("bIsTransparent", false);

	//Draw skybox last
	//Disable culling otherwise skyboxVAO is automatically removed
	glDisable(GL_CULL_FACE); 
	//glDepthMask(GL_FALSE);
	skyboxShader->use();
	skyboxShader->setMat4("projection", projection);
	//Remove translation from the view matrix 
	mat4 skyboxView = mat4(mat3(view));
	skyboxShader->setMat4("view", skyboxView);
	//Draw skybox cube
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//storing texture coordinates into buffer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Unbind VAO, EBO and VBO. MUST UNBIND VAO FIRST
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}
//STD
#include <string>
#include <map>
#include <algorithm>
using namespace std;

//GLEW
#define GLEW_STATIC
#include <GL\glew.h>

//GLFW
#include <GLFW\glfw3.h>

//GL : Shader.h | Model.h | Camera.h
#include "Camera.h"
#include "Shader.h"
#include "Model.h"

//GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

//OTHER : SOIL.h
#include <SOIL.h>

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720

#define DRAW_SIMPLE_TEST_TRIANGLE false

// Functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void DoMovement();
GLuint loadTexture(GLchar* path);

// Camera
Camera camera(glm::vec3(0.f, 0.f, 3.f));
bool keys[1024];
GLfloat lastX = SCREEN_WIDTH / 2, lastY = SCREEN_HEIGHT / 2;
bool firstMouse = true;

// Time
GLfloat deltaTime = 0.f;
GLfloat lastFrame = 0.f;

//-----------------------------------

float vertices[] = {
	-.5f, -.5f, 0.0f,
	.5f, -.5f, 0.0f,
	0.f,  .5f, 0.0f
};

//-----------------------------------

int main(char** argc, int argv)
{
	// Init GLFW and GL window hints
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window and context
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GL_Playground", nullptr, nullptr);
	if (!window)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Set callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Init GLEW 
	glewExperimental = GL_TRUE;
	glewInit();

	// Sett viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Load shaders

	Shader basicShader("Shaders/basic_shader.vs", "Shaders/basic_shader.frag");
	Shader nanosuitShader("Shaders/nanosuit_shader.vs", "Shaders/nanosuit_shader.frag");

	// Load models

	Model nanosuitModel("resources/objects/nanosuit/nanosuit.obj");
	glm::mat4 nModel;
	nModel = glm::translate(nModel, glm::vec3(0.f, -1.75f, 0.f));
	nModel = glm::scale(nModel, glm::vec3(0.2f, 0.2f, 0.2f));
	nanosuitModel.SetModelMatrix(nModel);

	//.
	//.
	//..

	// Map for all models to print with its shader associated
	map<int, pair<Shader*, Model*>> objects;
	objects[0] = pair<Shader*, Model*>(&nanosuitShader, &nanosuitModel);

	// Load simple triangle
	unsigned int VBO, VAO;
	if (DRAW_SIMPLE_TEST_TRIANGLE)
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}


	//Option: Draw wireframe.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glLineWidth(1.5f);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		DoMovement();

		// Clear buffs
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Light pos
		glm::vec3 lightPos = camera.position;

		for (auto it : objects)
		{
			Shader* currentShader = it.second.first;
			Model* currentModel = it.second.second;

			if (currentShader && currentModel)
			{
				currentShader->Use();

				glm::mat4 view = camera.GetViewMatrix();
				glm::mat4 proj = glm::perspective(camera.zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.f);
				
				glUniformMatrix4fv(glGetUniformLocation(currentShader->Program, "model"), 1, GL_FALSE, currentModel->GetModelMatrixPtr());
				glUniformMatrix4fv(glGetUniformLocation(currentShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(glGetUniformLocation(currentShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

				currentModel->Draw(*currentShader);

				glBindVertexArray(0);
				glUseProgram(0);
			}
		}

		if (DRAW_SIMPLE_TEST_TRIANGLE)
		{
			basicShader.Use();

			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.f, -1.25f, 0.f));
			glm::mat4 view = camera.GetViewMatrix();
			glm::mat4 proj = glm::perspective(camera.zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.f);

			glUniformMatrix4fv(glGetUniformLocation(basicShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(glGetUniformLocation(basicShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(basicShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glBindVertexArray(0);
			glUseProgram(0);
		}
		
		// Swap buffers
		glfwSwapBuffers(window);
	}

	// End glfw
	glfwTerminate();

	return 0;
}



// ----------------------------------------------------------
//		Functions definitions
// ----------------------------------------------------------

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.processKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.processKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.processKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.processKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_R])
		camera.processKeyboard(UP, deltaTime);
	if (keys[GLFW_KEY_F])
		camera.processKeyboard(DOWN, deltaTime);
}

// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return textureID;
}
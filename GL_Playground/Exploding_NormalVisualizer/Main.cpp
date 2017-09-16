#include <string>
#include <algorithm>
using namespace std;

#define GLEW_STATIC
#include <GL\glew.h>

#include <GLFW\glfw3.h>

#include "Camera.h"
#include "Shader.h"
#include "Model.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <SOIL.h>

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720

// Functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void DoMovement();
GLuint loadTexture(GLchar* path);

// Camera
Camera camera(glm::vec3(0.f, 0.f, 3.f));
bool keys[1024];
bool buttons[7];
GLfloat lastX = SCREEN_WIDTH / 2, lastY = SCREEN_HEIGHT / 2;
bool firstMouse = true;

// Time
GLfloat deltaTime = 0.f;
GLfloat lastFrame = 0.f;


int main(char** argc, int argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window and context
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GL_Playground: Geometry", nullptr, nullptr);
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
	glfwSetMouseButtonCallback(window, mouse_button_callback);

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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Load shaders
	Shader explodingShader("Shaders/exploding_shader.vs", "Shaders/exploding_shader.frag", "Shaders/exploding_shader.geom");
	Shader normalShader("Shaders/normal_visualizer_shader.vs", "Shaders/normal_visualizer_shader.frag", "Shaders/normal_visualizer_shader.geom");
	Shader textureShader("Shaders/texture_shader.vs", "Shaders/texture_shader.frag");

	//.
	//..
	//...

	// Loading models
	Model nano("resources/objects/nanosuit/nanosuit.obj");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//------------------------------------

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		explodingShader.Use();
		explodingShader.setMat4("projection", projection);
		explodingShader.setMat4("view", view);
		explodingShader.setMat4("model", model);

		// add time component to geometry shader in the form of a uniform
		explodingShader.setFloat("time", glfwGetTime());

		// draw model
		nano.Draw(explodingShader);
				
		// ===================================

		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
		textureShader.Use();
		textureShader.setMat4("projection", projection);
		textureShader.setMat4("view", view);
		textureShader.setMat4("model", model);

		nano.Draw(textureShader);

		normalShader.Use();
		normalShader.Use();
		normalShader.setMat4("projection", projection);
		normalShader.setMat4("view", view);
		normalShader.setMat4("model", model);

		nano.Draw(normalShader);

		//------------------------------------

		glfwSwapBuffers(window);
	}

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		buttons[button] = true;
	else
		buttons[button] = false;
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
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
#include "Ligth.h"
#include "Editor.h"

//GLM
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

//OTHER : SOIL.h
#include <SOIL.h>

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720

#define DRAW_SIMPLE_TEST_QUAD false

// Functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void DoMovement();
GLuint loadTexture(GLchar* path);

void Draw(unsigned int VAO, int size, Shader* sh, glm::vec3 pos, glm::vec3 scale, GLenum mode);
void DrawCube(unsigned int VAO, Shader* sh, glm::vec3 pos, glm::vec3 scale, GLenum mode, bool mustUseShader = true);

// Camera
Camera camera(glm::vec3(0.f, 0.f, 3.f));
bool keys[1024];
bool buttons[7];
GLfloat lastX = SCREEN_WIDTH / 2, lastY = SCREEN_HEIGHT / 2;
bool firstMouse = true;

// Time
GLfloat deltaTime = 0.f;
GLfloat lastFrame = 0.f;

// Editor bools
bool editorIsUsingMouse = false;
bool editorIsUsingKeyboard = false;


// Creating lights
DirectionalLight* dirLight = nullptr;

//-----------------------------------

float qVertices[] = {
	-.5f, -.5f, 0.0f,
	.5f, -.5f, 0.0f,
	0.5f,  .5f, 0.0f,
	-.5f, .5f, 0.0f,
	-.5f, -.5f, 0.0f,
	0.5f,  .5f, 0.0f
};

float cVertices[] = {
	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
	0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,		0.0f,  0.0f,  1.0f,		1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,		0.0f,  0.0f,  1.0f,		1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,		0.0f,  0.0f,  1.0f,		1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,

	0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,		0.0f, 1.0f,

	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f
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
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GL_Playground: Advanced GLSL", nullptr, nullptr);
	if (!window)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	Editor* editor = new Editor();
	editor->Init(window);

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
	glEnable(GL_PROGRAM_POINT_SIZE);

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);*/

	// Load shaders

	Shader basicShader("Shaders/basic_shader.vs", "Shaders/basic_shader.frag");
	Shader pointSizeShader("Shaders/point_size_shader.vs", "Shaders/point_size_shader.frag");
	Shader triangleShader("Shaders/triangle_shader.vs", "Shaders/triangle_shader.frag");
	Shader fragCoordShader("Shaders/frag_coord_shader.vs", "Shaders/frag_coord_shader.frag");
	Shader frontFacingShader("Shaders/front_facing_shader.vs", "Shaders/front_facing_shader.frag");
	Shader uniformBlockShader("Shaders/uniform_blocks_shader.vs", "Shaders/uniform_blocks_shader.frag");

	// Load models

	//.
	//.
	//..

	// Load individual textures
	GLuint checkerTexture = loadTexture("resources/textures/checker.jpg");
	GLuint matrixTexture = loadTexture("resources/textures/matrix.jpg");

	// Map for all models to print with its shader associated
	map<int, pair<Shader*, Model*>> objects;

	// Setting lights
	dirLight = new DirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));
	
	// Load simple triangle
	unsigned int qVBO, qVAO;
	glGenVertexArrays(1, &qVAO);
	glBindVertexArray(qVAO);

	glGenBuffers(1, &qVBO);
	glBindBuffer(GL_ARRAY_BUFFER, qVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(qVertices), qVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Load a cube
	unsigned int cVBO, cVAO;
	glGenVertexArrays(1, &cVAO);
	glGenBuffers(1, &cVBO);

	glBindVertexArray(cVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cVertices), cVertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// tex coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


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
		editor->NewFrame();
		editorIsUsingKeyboard = editor->editorIsUsingKeyboard;
		editorIsUsingMouse = editor->editorIsUsingMouse;
		DoMovement();

		// Clear buffs
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

				glUniform3f(glGetUniformLocation(currentShader->Program, "viewPos"), camera.position.x, camera.position.y, camera.position.z);

				currentModel->Draw(*currentShader, dirLight);

				glBindVertexArray(0);
				glUseProgram(0);
			}
		}

		// Print a quad's vvertex as points setting its point size from shader
		Draw(qVAO, 6, &pointSizeShader, glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), GL_POINTS);

		// Print a quad with colors according of frag coords
		Draw(qVAO, 6, &fragCoordShader, glm::vec3(0.0f, 0.f, 0.f), glm::vec3(1.0f, 1.0f, 1.0f), GL_TRIANGLES);

		// Print a cube
		frontFacingShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(frontFacingShader.Program, "checkers"), 0);
		glBindTexture(GL_TEXTURE_2D, checkerTexture);
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(frontFacingShader.Program, "matrix"), 1);
		glBindTexture(GL_TEXTURE_2D, matrixTexture);
		DrawCube(cVAO, &frontFacingShader, glm::vec3(-2.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), GL_TRIANGLES, false);

		// Print a triangle using in shader vertices // Not working properly
		/*triangleShader.Use();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 proj = glm::perspective(camera.zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.f);
		glm::mat4 model;
		glUniformMatrix4fv(glGetUniformLocation(triangleShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(triangleShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(triangleShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(0);*/

		if (DRAW_SIMPLE_TEST_QUAD)
		{
			Draw(qVAO, 6, &basicShader, glm::vec3(0.0f, -1.25f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), GL_TRIANGLES);
		}

		editor->Update();
		
		editor->Render();
		// Swap buffers
		glfwSwapBuffers(window);
	}

	editor->CleanUp();
	// End glfw
	glfwTerminate();

	delete dirLight;
	delete editor;

	return 0;
}



// ----------------------------------------------------------
//		Functions definitions
// ----------------------------------------------------------

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS && !editorIsUsingKeyboard)
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

	if(buttons[GLFW_MOUSE_BUTTON_LEFT] && !editorIsUsingMouse)
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

void Draw(unsigned int VAO, int size, Shader* sh, glm::vec3 pos, glm::vec3 scale, GLenum mode)
{
	sh->Use();

	glm::mat4 model;
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 proj = glm::perspective(camera.zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.f);

	glUniformMatrix4fv(glGetUniformLocation(sh->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(sh->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(sh->Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	glBindVertexArray(VAO);
	glDrawArrays(mode, 0, size);

	glBindVertexArray(0);
	glUseProgram(0);
}

void DrawCube(unsigned int VAO, Shader* sh, glm::vec3 pos, glm::vec3 scale, GLenum mode, bool mustUseShader)
{
	if(mustUseShader)
		sh->Use();

	glm::mat4 model;
	model = glm::translate(model, pos);
	model = glm::scale(model, scale);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 proj = glm::perspective(camera.zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.f);

	glUniformMatrix4fv(glGetUniformLocation(sh->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(sh->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(sh->Program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

	glBindVertexArray(VAO);
	glDrawArrays(mode, 0, 36);

	glBindVertexArray(0);
	glUseProgram(0);
}
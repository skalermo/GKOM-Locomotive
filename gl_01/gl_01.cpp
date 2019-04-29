#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <iomanip>

#include "Camera.h"
#include "Cylinder.h"
#include "Shader.h"
#include "Cube.h"
#include "Sphere.h"
#include "ThreeShapes.h"
#include "Text.h"


using namespace std;

typedef std::vector<std::shared_ptr<Shader>>  ShaderVector;

GLuint SCR_WIDTH;
GLuint SCR_HEIGHT;

void applyViewToShaders(ShaderVector shaders, const glm::mat4& projection, const glm::mat4& view);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
string doubleToString(double);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double lastX;
double lastY;
bool firstMouse = true;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	try
	{
		SCR_WIDTH = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
		SCR_HEIGHT = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GKOM - OpenGL 01", glfwGetPrimaryMonitor(), nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		// Set OpenGL options
		//glEnable(GL_CULL_FACE);

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		// tell GLFW to capture our mouse
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwGetCursorPos(window, &lastX, &lastY);

		auto shText = PrepareText(SCR_WIDTH, SCR_HEIGHT);

		auto shCylinder = Cylinder::getShaderPtr();
		auto shSphere = Sphere::getShaderPtr();
		auto shCube = Cube::getShaderPtr();

		auto threeShapes = ThreeShapes();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		double lastTime = lastFrame = glfwGetTime(), deltaT = 0.0, spf = 0.0;
		int nbFrames = 0;


		// main loop
		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			deltaT = currentFrame - lastTime;
			lastFrame = currentFrame;
			nbFrames++;
			// input
			// -----
			processInput(window);

			mouse_callback(window, lastX, lastY);
			//scroll_callback(window, 3, 3);
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions


			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (deltaT >= 0.25) { // If last prinf() was more than 1 sec ago
		 // printf and reset timer
				spf = 250 / (double)nbFrames;
				nbFrames = 0;
				lastTime += 0.25f;
			}

			RenderText(shText, "frame: " + doubleToString(spf) + "ms", 25.0f, SCR_HEIGHT - 20.0f, 0.4f, glm::vec3(1.0f));
			RenderText(shText, "FPS: " + std::to_string((int)(1000 / spf)), 25.0f, SCR_HEIGHT - 50.0f, 0.4f, glm::vec3(1.0f));
			RenderText(shText, "X=" + doubleToString(camera.Position.x) + "; Y=" + doubleToString(camera.Position.y) + "; Z=" + doubleToString(camera.Position.z), 25.0f, SCR_HEIGHT - 80.0f, 0.4f, glm::vec3(1.0f));



			auto projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			auto view = camera.GetViewMatrix();
			applyViewToShaders({ shCylinder, shCube, shSphere }, projection, view);

			threeShapes.move({ 0.001f, 0.0f, 0.0f });
			threeShapes.rotate({ 0.1f, 0.0f, 0.0f });
			threeShapes.draw();



			// Swap the screen buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	return 0;
}

void applyViewToShaders(ShaderVector shaders, const glm::mat4& projection, const glm::mat4& view)
{
	for (const auto& shader : shaders)
	{
		shader->use();
		shader->setTransformMatrix("projection", projection);
		shader->setTransformMatrix("view", view);
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

string doubleToString(double var)
{
	// Create an output string stream
	std::ostringstream streamObj;
	// Set Fixed -Point Notation
	streamObj << std::fixed;
	// Set precision to 2 digits
	streamObj << std::setprecision(2);
	//Add double to stream
	streamObj << var;
	return streamObj.str().c_str();
}

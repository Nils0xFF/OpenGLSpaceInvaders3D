#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <stdio.h>
#include "freeimage.h"
#include "Game.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "Random.h"

using namespace std;

// GLFW function declerations
void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mode);
void windowResize(GLFWwindow* window, int width, int height);
void toggleFullScreen(GLFWwindow* window);
void toggleMaxScreen(GLFWwindow* window);
void APIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam) {

	if (type == GL_DEBUG_TYPE_OTHER) return;

	cout << "---------------------opengl-callback-start------------" << endl;
	cout << "source: " << source << endl;
	cout << "message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;

	cout << "id: " << id << endl;
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	}
	cout << endl;
	cout << "---------------------opengl-callback-end--------------" << endl;
}

int WINDOWPOS_X = 100;
int WINDOWPOS_Y = 100;
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
int SAVED_WINDOW_WIDTH = WINDOW_WIDTH;
int SAVED_WINDOW_HEIGHT = WINDOW_HEIGHT;
bool isFullscreen = false;
bool isMaxed = false;

Game* myGame;

int main() {
	FreeImage_Initialise();
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Speice Inveiders", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: can not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwSetWindowPos(window, WINDOWPOS_X, WINDOWPOS_Y);
	glfwMakeContextCurrent(window);

#if WIN32
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	glGetError();
	glfwSetKeyCallback(window, keyPressed);
	glfwSetFramebufferSizeCallback(window, windowResize);


	// OpenGL config
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	myGame = new Game(window);
	// Initialize the Game
	myGame->Init();
	// Start the Game
	myGame->Start();

	Random::Init();

	GLfloat lastTime = 0;
	while (!glfwWindowShouldClose(window))
	{
		// used instead of glfwSetMaximizeCallback doesnt exist
		if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
			isMaxed = true;
		}

		// delta time calulations
		GLfloat now = (GLfloat)glfwGetTime();
		GLfloat delta = now - lastTime;
		lastTime = now;

		// once per frame
		glfwPollEvents();
		myGame->ProcessInput(delta);

		// update Game
		myGame->Update(delta);

		// render Game
		myGame->Render();

		glfwSwapBuffers(window);
	}


	myGame->End();
	delete myGame;


	glfwTerminate();
	return 0;
}

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	InputManager& inputManager = InputManager::getInstance();
	if ((key == GLFW_KEY_KP_0 || key == GLFW_KEY_0) && action == GLFW_PRESS)
		PostProcessing::getInstance().shader->on(!PostProcessing::getInstance().shader->isOn());
	if ((key == GLFW_KEY_KP_1 || key == GLFW_KEY_1) && action == GLFW_PRESS)
		PostProcessing::getInstance().shader->fog(!PostProcessing::getInstance().shader->isFog());
	if ((key == GLFW_KEY_KP_2 || key == GLFW_KEY_2) && action == GLFW_PRESS)
		PostProcessing::getInstance().shader->bloom(!PostProcessing::getInstance().shader->isBloom());
	if ((key == GLFW_KEY_KP_3 || key == GLFW_KEY_3) && action == GLFW_PRESS)
		PostProcessing::getInstance().shader->inverted(!PostProcessing::getInstance().shader->isInverted());
	if ((key == GLFW_KEY_KP_4 || key == GLFW_KEY_4) && action == GLFW_PRESS)
		PostProcessing::getInstance().shader->gray(!PostProcessing::getInstance().shader->isGray());
	if ((key == GLFW_KEY_KP_5 || key == GLFW_KEY_5) && action == GLFW_PRESS)
		PostProcessing::getInstance().setMSAA(1);
	if ((key == GLFW_KEY_KP_6 || key == GLFW_KEY_6) && action == GLFW_PRESS)
		PostProcessing::getInstance().setMSAA(2);
	if ((key == GLFW_KEY_KP_7 || key == GLFW_KEY_7) && action == GLFW_PRESS)
		PostProcessing::getInstance().setMSAA(4);
	if ((key == GLFW_KEY_KP_8 || key == GLFW_KEY_8) && action == GLFW_PRESS)
		PostProcessing::getInstance().setMSAA(8);
	if ((key == GLFW_KEY_KP_9 || key == GLFW_KEY_9) && action == GLFW_PRESS)
		PostProcessing::getInstance().setMSAA(16);
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
		toggleFullScreen(window);
	if (key == GLFW_KEY_F10 && action == GLFW_PRESS)
		toggleMaxScreen(window);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			inputManager.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			inputManager.Keys[key] = GL_FALSE;
	}
}

void windowResize(GLFWwindow* window, int width, int height)
{
	if (width > 0 && height > 0) {
		WINDOW_WIDTH = (int)(height * 16 / 9);
		WINDOW_HEIGHT = height;
		if (width < WINDOW_WIDTH) {
			WINDOW_WIDTH = width;
			WINDOW_HEIGHT = (int)(width * 9 / 16);
		}
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		CameraManager::getInstance().activeCamera->updateResolution(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
	else {
		GameManager::getInstance().PauseGame();
	}
}

void toggleFullScreen(GLFWwindow* window)
{
	isFullscreen = !isFullscreen;
	if (isFullscreen)
	{
		glfwGetWindowPos(window, &WINDOWPOS_X, &WINDOWPOS_Y);
		glfwGetWindowSize(window, &SAVED_WINDOW_WIDTH, &SAVED_WINDOW_HEIGHT);

		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);
	}
	else
	{
		glfwSetWindowMonitor(window, nullptr, WINDOWPOS_X, WINDOWPOS_Y, SAVED_WINDOW_WIDTH, SAVED_WINDOW_HEIGHT, 0);
	}
}

void toggleMaxScreen(GLFWwindow* window)
{
	if (isFullscreen) toggleFullScreen(window);

	isMaxed = !isMaxed;
	if (isMaxed)
	{
		glfwMaximizeWindow(window);
	}
	else
	{
		glfwRestoreWindow(window);
	}
}

void debugCallback() {

}
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


// GLFW function declerations
void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mode);
void windowResize(GLFWwindow* window, int width, int height);
void windowMaximize(GLFWwindow* window, int maximized);
void toggleFullScreen(GLFWwindow* window);
void toggleMaxScreen(GLFWwindow* window);

int WINDOWPOS_X = 100;
int WINDOWPOS_Y = 100;
int WINDOW_WIDTH = 960;
int WINDOW_HEIGHT = 540;
int SAVED_WINDOW_WIDTH = WINDOW_WIDTH;
int SAVED_WINDOW_HEIGHT = WINDOW_HEIGHT;
bool isFullscreen = false;
bool isMaxed = false;

Game* myGame;

int main () {
    FreeImage_Initialise();
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
#ifdef __APPLE__
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    	
    GLFWwindow* window = glfwCreateWindow (WINDOW_WIDTH, WINDOW_HEIGHT, "Speice Inveiders", NULL, NULL);
    if (!window) {
        fprintf (stderr, "ERROR: can not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwSetWindowPos(window, WINDOWPOS_X, WINDOWPOS_Y);
    glfwMakeContextCurrent (window);

#if WIN32
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	glGetError();
	glfwSetKeyCallback(window, keyPressed);
	glfwSetFramebufferSizeCallback(window, windowResize);    

	// OpenGL config
	glViewport(0, 0, 960, 540);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	myGame = new Game(window);
	// Initialize the Game
	myGame->Init();
	// Start the Game
	myGame->Start();    
    
    
	GLfloat lastTime = 0;
    while (!glfwWindowShouldClose (window))
    {
        // used instead of glfwSetMaximizeCallback doesnt exist
        if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
            isMaxed = true;
        }

		// delta time calulations
		GLfloat now = glfwGetTime();
		GLfloat delta = now - lastTime;
        lastTime = now;

        // once per frame
        glfwPollEvents();
        myGame->ProcessInput(delta);

		// update Game
        myGame->Update(delta);

		// render Game
		myGame->Render();        
		
        glfwSwapBuffers (window);
    }
    
	
	myGame->End();
	delete myGame;
   
    
    glfwTerminate();
    return 0;
}

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	InputManager& inputManager = InputManager::getInstance();
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_KP_0 && action == GLFW_PRESS)
		PostProcessing::getInstance().shader->on(!PostProcessing::getInstance().shader->isOn());
    if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
        PostProcessing::getInstance().setMSAA(1);
    if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS)
        PostProcessing::getInstance().setMSAA(32);
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
    WINDOW_WIDTH = (int)(height * 16 / 9);
    WINDOW_HEIGHT = height;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	myGame->WindowResize(WINDOW_WIDTH, WINDOW_HEIGHT);
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
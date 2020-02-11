#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <stdio.h>
#include "Application.h"
#include "freeimage.h"
#include "Game.h"

void PrintOpenGLVersion();

// GLFW function declerations
void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const GLuint SCREEN_WIDTH = 800;

// The height of the screen
const GLuint SCREEN_HEIGHT = 600;

Game myGame(SCREEN_WIDTH, SCREEN_HEIGHT);

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
    

    
    GLFWwindow* window = glfwCreateWindow (SCREEN_WIDTH, SCREEN_HEIGHT, "Speice Inveiders", NULL, NULL);
    if (!window) {
        fprintf (stderr, "ERROR: can not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent (window);

#if WIN32
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	myGame.Init();
    PrintOpenGLVersion();
    
    {
        double lastTime=0;
        while (!glfwWindowShouldClose (window)) {
            double now = glfwGetTime();
            double delta = now - lastTime;
            lastTime = now;
            // once per frame
            glfwPollEvents();
			myGame.ProcessInput((float) delta);
            myGame.Update((float)delta);
            glfwSwapBuffers (window);
        }
        myGame.End();
    }
    
    glfwTerminate();
    return 0;
}


void PrintOpenGLVersion()
{
    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);
}

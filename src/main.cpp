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

// Post-Processing
#include "BaseShader.h"
#include "CameraManager.h"
#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

// GLFW function declerations
void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mode);
void windowResize(GLFWwindow* window, int width, int height);

// The Width of the screen
const GLuint SCREEN_WIDTH = 960;

// The height of the screen
const GLuint SCREEN_HEIGHT = 540;

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

	glGetError();
	glfwSetKeyCallback(window, keyPressed);
	glfwSetWindowSizeCallback(window, windowResize);

	// OpenGL config
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Post-Processing
	// Framebuffer for off-screen rendering
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Texture-attachment for framebuffer
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Attaching texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	// Renderbuffer to sample depth- & stencil-texture
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attaching renderbuffer (depth- & stencil-texture)
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check for complete framebuffer init
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Not able to create framebuffer." << std::endl;
		return -1;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// TODO REMOVE
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	BaseShader* screenShader = new BaseShader();
	screenShader->load(ASSET_DIRECTORY "vsscreen.glsl", ASSET_DIRECTORY "fsscreen.glsl");


	Game myGame(window);
	// Initialize the Game
	myGame.Init();
	// Start the Game
	myGame.Start();
    
	// Post-Processing Test
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
	GLfloat lastTime = 0;
	GLfloat combinedTime = 0;
    while (!glfwWindowShouldClose (window)) {
		
		// delta time calulations
		GLfloat  now = glfwGetTime();
		GLfloat  delta = now - lastTime;
        lastTime = now;

        // once per frame
        glfwPollEvents();

		// update Game
        myGame.Update(delta);

		// render Game
		

		// Post-Processing
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		myGame.Render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		screenShader->activate(*CameraManager::getInstance().activeCamera);
		screenShader->setParameter(screenShader->getParameterID("time"), now);
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers (window);
    }
    
	
	myGame.End();

	// Post-Processing
	glDeleteFramebuffers(1, &fbo);
   
    
    glfwTerminate();
    return 0;
}

void keyPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	InputManager& inputManager = InputManager::getInstance();
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
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
	int newWidth = (int)(height * 16 / 9);
	glViewport(0, 0, newWidth, height);
}
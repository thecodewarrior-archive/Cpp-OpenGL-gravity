#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>

#include "render.cpp"
Render render;

#define MIN_TIMESTEP 1/60.0

using namespace glm;

float mouseX, mouseY;

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        render.mouseDown(button, mods);
    if (action == GLFW_RELEASE)
        render.mouseUp(button, mods);
}
static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    mouseX = xpos;
    mouseY = ypos;
    render.mousePos(mouseX, mouseY);
}
static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    render.mouseWheel(xoffset, yoffset);
}
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        render.keyDown(key, scancode, mods);
    }
    if(action == GLFW_RELEASE) {
        render.keyUp(key, scancode, mods);
    }
}
int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
    glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    render.init();
    
    double time = glfwGetTime();
    
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
	do{
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );
        
        render.draw();
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        double currentTime = glfwGetTime();
        render.update(min(MIN_TIMESTEP, currentTime - time));
        time = currentTime;
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
    
    render.clean();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

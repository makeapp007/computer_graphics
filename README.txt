# computer_graphics
backup code for computer graphics 

useful tutorial
https://github.com/JoeyDeVries/LearnOpenGL


###ATTENTION###
The code here pass compilation on mac 
	it behaves differently on windows when using opengl and glfw
if you are using opengl, pay attention to the library including order 
'''
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
// GLFW
#include <GLFW/glfw3.h>
'''
and initialing glfw order
'''
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
'''



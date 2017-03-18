#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
// GLFW
#include <GLFW/glfw3.h>


#include "SOIL/src/SOIL.h"


// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>


// Other includes
#include "Shader.h"
using namespace std;



// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 0.0f);\n"
    "}\n\0";


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if(key == GLFW_KEY_D && action == GLFW_PRESS){
		cout<<"start drawing saber"<<endl;
	}
	else if(key == GLFW_KEY_S && action == GLFW_PRESS){
		cout<<"start receiving input"<<endl;
	}	
}


int times=0;
float saberCoor[8]; //2 for one point,(x,y)
int saberindex=0;
int saberindex_flag=0;
int currentObject=0;


// static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
// 	// int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
// 	// if (state == GLFW_PRESS){
// 	// 	cout<<"i am in cursor callback function  "<<"x is "<<xpos<<" y is  "<< ypos	<<"   " <<times<<endl;
// 	// 	times++;
// 	// 	float NDCx=(xpos-400)/400;
// 	// 	float NDCy=(ypos-300)/300;
// 	// 	saberCoor[saberindex]=NDCx;
// 	// 	saberindex++;
// 	// 	saberCoor[saberindex]=NDCy;
// 	// 	saberindex++;
// 	// 	cout<<"currently "<<"x is "<<NDCx<<" y is  "<< NDCy	<<"   " <<times<<endl;


// 	// }

// }

GLuint VBO1, VAO1, EBO1;
GLfloat vertices1[] = {
	// Positions          // Colors           // Texture Coords
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
};

GLuint indices1[] = {  // Note that we start from 0!
	0, 1, 2, // First Triangle
	// 1, 2, 3  // Second Triangle
};


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        cout<<"mouse  button is "<<button<<" action is "<<action<<" mods is "<<mods<<endl;
        double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		float NDCx=0;
		float NDCy=0;
		if(xpos<=800 && xpos>=0 && ypos<=600 && ypos>=0){
			NDCx=(xpos-400)/400;
			NDCy=-(ypos-300)/300;
		}
		// cout<<"i am in cursor callback function  "<<"x is "<<xpos<<" y is  "<< ypos	<<"   "<<endl;

		// store four points's coordinate
		if(saberindex<9){

			// saberCoor[saberindex]=NDCx;
			// saberindex++;
			// saberCoor[saberindex]=NDCy;
			// saberindex++;
			vertices1[saberindex]=(GLfloat)NDCx;
			saberindex++;
			vertices1[saberindex]=(GLfloat)NDCy;
			saberindex++;
			vertices1[saberindex]=(GLfloat)0;
			saberindex++;
			// cout<<(GLfloat)NDCx<<" (GLfloat)NDCx"<<endl;
			cout<<"currently "<<"x is "<<NDCx<<" y is  "<< NDCy	<<"  current saberindex is  "<<saberindex <<endl;
		}
		else if(saberindex==9){
			cout<<"*****dangerous, this array only allows to store four points*****"<<endl;
			cout<<"if you click again, it will draw a rectangle"<<endl;
			saberindex++;

		}
		else if(saberindex==10){
			saberindex_flag=1;
			saberindex++;

		}


    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
    	cout<<"mouse  button is "<<button<<" action is "<<action<<" mods is "<<mods<<endl;
		cout<<"finish drawing"<<endl;
    }
}




// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Texture", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	// glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);



// color shader
		glGenVertexArrays(1, &VAO1);
		glGenBuffers(1, &VBO1);
		glGenBuffers(1, &EBO1);

		glBindVertexArray(VAO1);

		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_DYNAMIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0); // Unbind VAO1



    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);




	// Build and compile our shader program
	Shader ourShader("texture.vs", "texture.frag");



	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
		-1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};


	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	glBindVertexArray(0); // Unbind VAO

	GLuint texture1;
	GLuint texture2;


	//texture1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	//if u use openCV:
	/*cv::Mat myMat = cv::imread("container.jpg");
	height = myMat.rows;
	width = myMat.cols;
	cv::Mat image;
	cv::flip(myMat, image, 0);*/
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	//texture2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("flag.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if(currentObject==0){
			// cout<<"currentObject is "<<currentObject<<endl;
			// bindTexture1
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

		}
			// cout<<"currentObject is "<<currentObject<<endl;
			// //bindTexture2
			// glActiveTexture(GL_TEXTURE1);
			// glBindTexture(GL_TEXTURE_2D, texture2);
			// glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);


		// }
		// if(saberindex==9 && saberindex_flag==1){

			    // Draw our first triangle


		// repaint the picture
		if(saberindex_flag==1){	
			glGenVertexArrays(1, &VAO1);
			glGenBuffers(1, &VBO1);
			glGenBuffers(1, &EBO1);

			glBindVertexArray(VAO1);

			glBindBuffer(GL_ARRAY_BUFFER, VBO1);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_DYNAMIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0); // Unbind VAO1
			saberindex_flag=0;

		}

		// }
			// // Draw the triangle

			// first texture, then shader. otherwise, not work
			ourShader.Use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

	        glUseProgram(shaderProgram);
	        glBindVertexArray(VAO1);
	        //glDrawArrays(GL_TRIANGLES, 0, 6);
	        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	        glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


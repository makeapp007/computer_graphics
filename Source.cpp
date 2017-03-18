#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
// GLFW
#include <GLFW/glfw3.h>
#include <string>

#include "SOIL/src/SOIL.h"
#include <fstream>


#include "Shader.h"
using namespace std;
// using namespace cv;



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


double timeToswitch =0; //time to switch image
int saberindex=0;  //current picture's point's index
int saberindex_flag=0;
int currentObject=0;   
int imageNum=87;	//total number of image
int imageNumindex=1;  //which image to display
int step=5;  //calibrate every 5 pictures
int changePicture= 0;
int pointNum=0;  //if equals 4, then paint picture and change picture.
int paintPicture=0;



GLuint VBO1, VAO1, EBO1;
GLfloat vertices1[3*4];
// GLfloat verticesall[87*3*4];
// int* p1 = (int*)std::malloc(4*sizeof(int));  // allocates enough for an array of 4 int
//     int* p2 = (int*)std::malloc(sizeof(int[4])); // same, naming the type directly
//     int* p3 = (int*)std::malloc(4*sizeof *p3);   // same, without repeating the type name
 
GLfloat* verticesall= (GLfloat*)malloc(sizeof(GLfloat[87*3*4]));
//  if not use malloc, encounter memory problem(like bus error)

// vertices1[] = {
// 	// Positions          // Colors           // Texture Coords
// 	0.5f, 0.5f, 0.0f,
// 	0.5f, -0.5f, 0.0f,
// 	-0.5f, -0.5f, 0.0f,
// 	-0.5f, 0.5f, 0.0f,
// };

void copySaberindex(){
	// cout<<"error occurs hereaaa "<<imageNumindex<<"  "<<(imageNumindex-1)*60<<endl;
	for(int i=0;i<12;i++){
		verticesall[i+(imageNumindex-1)/5*60]=vertices1[i];
	}
	// cout<<"error occurs here"<<endl;
}
void restoreVertex(){
	for(int i=0;i<12;i++){
		vertices1[i]=verticesall[i+(imageNumindex-1)/5*60];
	}
}


void initialSaberVertex(){
	for(int i=0;i<12;i++){
		vertices1[i]=(GLfloat)0.0f;
	}
	// cout<<"error occurs herein he "<<endl;

}


GLuint indices1[] = {  // Note that we start from 0!
	0, 1, 2, // First Triangle
	0, 2, 3  // Second Triangle
};



// point 4 times to record four points, fifth time to paint rectangle, sixth time to change picture and erase rectangle
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
		else{
			return ;
		}

		// pointing how many times 
		pointNum++;
		// store four points's coordinate
		if(saberindex<12){

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
		else if(pointNum==5 && changePicture==0 ){
			paintPicture=1;
			// saberindex_flag=1;
			// cout<<"changePicture=0;  "<<saberindex_flag<<endl;
		}
		else if(pointNum==6 && changePicture==0 ) {
			pointNum=0;
			changePicture=1;
			// cout<<"changePicture=1; "<<saberindex_flag<<endl;
		}


    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
    	cout<<"in GLFW_MOUSE_BUTTON_RIGHT";
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Computer Graphics HW1", nullptr, nullptr);
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
	// this is for the color block
	Shader colorShader("shader.vs", "shader.frag");
	// this is for the video clip
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
	// int width, height;
	// unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// glGenerateMipmap(GL_TEXTURE_2D);
	// SOIL_free_image_data(image);
	// glBindTexture(GL_TEXTURE_2D, 0);

		string imagepath="image-"+to_string(imageNumindex)+".jpeg";
		// cout<<"imagepath is "<<imagepath<<endl;
				int width, height;	
		unsigned char* image = SOIL_load_image(imagepath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);


	//texture2
	// glGenTextures(1, &texture2);
	// glBindTexture(GL_TEXTURE_2D, texture2);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// image = SOIL_load_image("flag.png", &width, &height, 0, SOIL_LOAD_RGB);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	// glGenerateMipmap(GL_TEXTURE_2D);
	// SOIL_free_image_data(image);
	// glBindTexture(GL_TEXTURE_2D, 0);




	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// change texture every second
		// if(currentObject==0){
			// cout<<"currentObject is "<<currentObject<<endl;
			// bindTexture1
		// unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);

		if(changePicture==1){

			copySaberindex();
			initialSaberVertex();
			imageNumindex+=step;
			if(imageNumindex>imageNum){
				break;
				// start to interpolate

			}
			saberindex=0;
			changePicture=0;

			// change picture, this picture is used in texture
			string imagepath="image-"+to_string(imageNumindex)+".jpeg";
			cout<<"------imagepath is "<<imagepath<<endl;
			image = SOIL_load_image(imagepath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);		
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);


			// notify to erase shader
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

		}
		// cout<<"   "<<imagesequence<< "width is "<<width <<endl;


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);


		// repaint the picture and display the effect
		if(paintPicture==1){	
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
			paintPicture=0;

		}



			// first texture, then shader. otherwise, not work
			ourShader.Use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			colorShader.Use();
	        // glUseProgram(shaderProgram);
	        glBindVertexArray(VAO1);
	        //glDrawArrays(GL_TRIANGLES, 0, 6);
	        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	        glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	



	ofstream outfile;
	outfile.open("data_ori.txt");
	// write im to file
	for(int i=0;i<imageNum*12;i++){
		outfile << verticesall[i]<< " ";
		if(i>0 && i%12==0){
			outfile<<endl;
		}
	}
	cout<<"finish writing vertice origin to local storage"<<endl;



	// **************
	// start to interpolate
	// **************
	imageNumindex=1;
	float dis=0;
	for(;imageNumindex<imageNum-step;imageNumindex+=step){

		// each 12 values
		// i=0,3,6,9
		for(int i=0;i<12;i=i+3){
			// 0.x
			dis=verticesall[60*(imageNumindex+4)/5+i]-verticesall[60*(imageNumindex-1)/5+i];
			dis=dis/5;
			// j is 1,2,3,4
			for(int j=1;j<5;j++){
				verticesall[60*(imageNumindex+4)/5+i+j*12]=verticesall[60*(imageNumindex-1)/5+i]+dis*j;
			}
			// 0.y
			dis=verticesall[60*(imageNumindex+4)/5+i+1]-verticesall[60*(imageNumindex-1)/5+i+1];
			dis=dis/5;
			for(int j=1;j<5;j++){
				verticesall[60*(imageNumindex+4)/5+i+1+j*12]=verticesall[60*(imageNumindex-1)/5+i+1]+dis*j;
			}
			// 0.z
			for(int j=1;j<5;j++){
				verticesall[60*(imageNumindex+4)/5+i+2+j*12]=0;
			}

		}
	}	

	ofstream outfile2;
	outfile2.open("data_after.txt");
	// write im to file, print every 12 value
	for(int i=0;i<imageNum*12;i++){
		outfile2 << verticesall[i]<< " ";
		if(i>0 && i%12==0){
			outfile2<<endl;
		}
	}



	// start streaming like a video
	imageNumindex=1;
	// saberindex=0;
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		

		// replay again
		if(imageNumindex>imageNum){
			imageNumindex=1;
		}


		timeToswitch = glfwGetTime();
		while(1){
			// 1/30=0.03	
			if(glfwGetTime()-timeToswitch >0.03){
				cout<<"switching image"<<endl;
				break;
			}
		}

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// restore vertice1
		restoreVertex();
		// paint the rectangle
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



		// draw texture
		string imagepath="image-"+to_string(imageNumindex)+".jpeg";
		// cout<<"------imagepath is "<<imagepath<<endl;
		image = SOIL_load_image(imagepath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);


		// active texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

		// paint shader
			// first texture, then shader. otherwise, not work
			ourShader.Use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			colorShader.Use();
	        glBindVertexArray(VAO1);
	        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	        glBindVertexArray(0);

	    imageNumindex++;
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	




	// ********
	// end of code
	// ********



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


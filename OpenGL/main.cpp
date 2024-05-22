#include <iostream>
#include <glad/glad.h>
#include <cmath>
#include <GLFW/glfw3.h>
#include "shader.hpp"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Normalized Device Coordinates 
//-----------------------------
float vertices[] = {
	//positions			//colors
	 0.95f,  0.8f, 0.0f, 1.0f,0.0f,0.0f,
	 -0.3f, -0.2f, 0.0f,  0.0f, 1.0f, 0.0f,
	 0.95f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};
float vertices2[] = {
	-0.95f, 0.5f, 0.0f,  // top right
	 0.1f, -0.5f, 0.0f,  // bottom right
	-0.95f, -0.5f, 0.0f
};

//unsigned int indices[] = {  // note that we start from 0!
//	0, 1, 3,   // first triangle
//	1, 2, 3    // second triangle
//};

//Vertex Buffer Object to be generated
unsigned int VBO[2];
unsigned int VAO[2];

unsigned int EBO;

unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int fragmentShader2;

unsigned int shaderProgram;
unsigned int shaderProgram2;

int main() {

	//glfw initialization and configuration
	//-------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//glfw window creation
	//--------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OPENGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glad loading all OpenGL function pointers
	//-----------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//Vertex shader compiling
	Shader shader("shader.vs", "fragment.fs");


	//2 VAO and VBOs are being used
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	//EBO generation
	/*glGenBuffers(1, &EBO);*/

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	//First VAO
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//EBO binding
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);*/
	
	//Second VAO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// EBO STUFF
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	//Main render loop
	//----------------
	while (!glfwWindowShouldClose(window)) {
		//Process escape input
		//--------------------
		processInput(window);
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

		//Rendering color
		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime()*3;
		float greenValue = (sin(timeValue) / 3.0f) + .5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

		//Rendering Triangle
		shader.use();

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		shader.use();


		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glfw swap buffers and poll IO events
		//------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO[0]);
	glDeleteBuffers(1, &VBO[0]);
	glDeleteVertexArrays(1, &VAO[1]);
	glDeleteBuffers(1, &VBO[1]);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

//resizing on buffer
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

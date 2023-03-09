#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "../../../includes/learnopengl/shader_s.h"

using namespace std;

// callback method for change in window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// process inputs in each iteration of render loop
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	// Set OpenGL version to 3.3 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// for Mac OS X, uncomment the following line
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// create GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// check wheter GLAD has initialized successfully
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// inform OpenGL about window size and set callback when it's changed
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// create shader
	Shader shader("6.4.shader.vs", "6.4.shader.fs");
	
	// set up vertex data

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	// bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f	// top
	};

	// define vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// define vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	
	// bind the VAO
	glBindVertexArray(VAO);

	// copy the vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// let OpenGL know how to interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// Unbind the VAO so it won't be accidentally modified by other VAO calls
	glBindVertexArray(0); 

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// redering commands
		
		// clear the color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate the shader program
		shader.use();

		// update the uniform offset
		float timeValue = glfwGetTime();
		float offset = sin(timeValue)/2.0f;
		shader.setFloat("xOffset", offset);

		// bind the vertex array object
		glBindVertexArray(VAO);
		// draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap the buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

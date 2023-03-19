#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../../../includes/stb_image.h"

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

// draw fractal
void recursive_draw(Shader shader, glm::mat4 trans, float scale, int depth)
{
	if (depth == 0)
	return;

	unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	// draw the triangles
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0f));
	// trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 trans_top = glm::translate(trans, glm::vec3(0, scale, 0.0f));
	trans_top = glm::scale(trans_top, glm::vec3(scale));
	recursive_draw(shader, trans_top, scale, depth - 1);

	glm::mat4 trans_right = glm::translate(trans, glm::vec3(scale, -scale, 0.0f));
	trans_right = glm::scale(trans_right, glm::vec3(scale));
	recursive_draw(shader, trans_right, scale, depth - 1);

	glm::mat4 trans_left = glm::translate(trans, glm::vec3(-scale, -scale, 0.0f));
	trans_left = glm::scale(trans_left, glm::vec3(scale));
	recursive_draw(shader, trans_left, scale, depth - 1);
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
	Shader shader("8.4.transform.vs", "8.4.transform.fs");
	
	// set up vertex data

	float vertices[] = {
		// positions         // colors		   // texture coords
		-0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f, -0.5f,  1.0f,	// top left
		 0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  1.5f,  1.0f,	// top right
		 0.0f,-1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.5f, -0.5f	// bottom
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	// define vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// define vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// define element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	
	// bind the VAO
	glBindVertexArray(VAO);

	// copy the vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// copy the indices array in a buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// let OpenGL know how to interpret the vertex data
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	// Unbind the VAO so it won't be accidentally modified by other VAO calls
	glBindVertexArray(0);

	// load and create a texture
	// -------------------------
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// set the texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, heigth, nrChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("../../../resources/textures/awesomeface.png", &width, &heigth, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE_IMAGE" << endl;
	}
	stbi_image_free(data);

	// to use background color where texture is transparent 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// redering commands
		
		// clear the color buffer
		glClearColor(0.8f, 0.75f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate the shader program
		shader.use();

		// bind the vertex array object
		glBindVertexArray(VAO);
		
		// bind the textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// draw sierpinski triangle fractal
		glm::mat4 trans = glm::mat4(1.0f);
		recursive_draw(shader, trans, 0.5, 7);

		// swap the buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they-ve outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clean all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}

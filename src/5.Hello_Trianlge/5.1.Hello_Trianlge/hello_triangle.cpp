#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

const char* vertexShaderSource =	"#version 330 core\n"
									"layout(location = 0) in vec3 aPos;\n"
									"void main()\n"
									"{\n"
									"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
									"}\0";

const char* fragmentShaderSource = 	"#version 330 core\n"
									"out vec4 FragColor;\n"
									"\n"
									"void main()\n"
									"{\n"
									"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
									"}\0";



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

	// create vertex shader and compile it
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	// check if compilation of vertex shader was successful
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	// create fragment shader and compile it
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check if compilation of fragment shader was successful
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	// create shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// attach shaders to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check if linking the shader program was successful
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILDED\n" << infoLog << endl;
	}

	// delete the shader objects after linking them into the program is done
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Unbind the VAO so it won't be accidentally modified by other VAO calls
	glBindVertexArray(0); 

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// redering commands
		
		// clear the scene
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// activate the shader program
		glUseProgram(shaderProgram);
		// bind the vertex array object
		glBindVertexArray(VAO);
		// draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

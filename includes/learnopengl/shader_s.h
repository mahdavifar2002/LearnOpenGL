#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
	// the program ID
	unsigned int ID;

	// constructor reads and buildes the shader
	Shader (const char *vertexPath, const char *fragmentPath)
	{
		// 1. retrive the vertex/fragment source code from filePath
		string vertexCode, fragmentCode;
		ifstream vShaderFile, fShaderFile;
		// ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		try
		{
			// open files
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				stringstream vShaderStream, fShaderStream;
				// read file's buffer contents into streams
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				// close file handlers
				vShaderFile.close();
				fShaderFile.close();
				// convert streams into strings
				vertexCode   = vShaderStream.str();
				fragmentCode = fShaderStream.str();
		}
		catch (ifstream::failure const &e)
		{
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
		}
		const char *vShaderCode = vertexCode.c_str();
		const char *fShaderCode = fragmentCode.c_str();

		// 2. compile the shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shader objects after they're linked into our program
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}
	// activate the shader program
	void use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	void setBool(const string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void set4Float(const string &name, float v0, float v1, float v2, float v3) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
	}

private:
	// utility function for checking shader compilation/linking errors
	void checkCompileErrors(unsigned int shader, string type)
	{
		int success;
		char infoLog[1024];

		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED" << endl;
				cout << infoLog << endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "ERROR::SHADER::" << type << "::LINKING_FAILED" << endl;
				cout << infoLog << endl;
			}
		}
	}
};


#endif
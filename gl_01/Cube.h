#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "Object.h"
#include "ShaderProvider.h"
#include "Shader.h"
#include "SOIL.h"

class Cube : Object
{
private:
	glm::vec3 coordinates; 
	GLfloat Xsize, Ysize, Zsize; 


	GLfloat vertices[24] = {
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0
	}; 

	GLuint indices[36] = {
		0, 1, 5,
		0, 4, 5,
		0, 1, 3,
		0, 2, 3,
		0, 4, 6,
		0, 2, 6,
		7, 6, 2,
		7, 3, 2,
		7, 5, 1,
		7, 3, 1,
		7, 5, 4,
		7, 6, 4
	};



public : 
	void print()
	{
		for (int i = 0; i < 36; i++)std::cout << indices[i] << std::endl;
	}

	Cube(int test)
	{
		print(); 
	}
	
	virtual void move(glm::vec3 displacement)
	{

	}

	virtual void draw()
	{

	}
};

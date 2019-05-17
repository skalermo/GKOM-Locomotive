#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ShaderProvider.h"
#include <math.h>
#include "Primitive.h"

class Cube : public Primitive
{
private:
	GLuint texture0; 
	std::shared_ptr<Shader> shader; 
	const std::string texturePath; 
	Texture texture; 

	void insertVert(glm::vec3 v, glm::vec3 n)
	{
		vertices.push_back(v.x);
		vertices.push_back(v.y); 
		vertices.push_back(v.z);
		vertices.push_back(n.x); 
		vertices.push_back(n.y); 
		vertices.push_back(n.z); 
		int gray = (((vertices.size() - 1) / 8 % 4) >> 1) ^ (((vertices.size() - 1) / 8 % 4));
		
		vertices.push_back((GLfloat)(gray >> 1));
		vertices.push_back((GLfloat)(gray %2));
	}

	void generateWall(glm::vec3 normal)
	{
		glm::vec3 firstPoint; 
		firstPoint.x = normal.x == 0 ? -1 : normal.x;
		firstPoint.y = normal.y == 0 ? -1 : normal.y; 
		firstPoint.z = normal.z == 0 ? 1 : normal.z; 

		for (int i = 0; i < 4; i++)
		{
			insertVert(firstPoint, normal); 
			firstPoint = cross(firstPoint, normal) + normal;
		}
	}
	void generateVertices() override
	{
		generateWall({ -1, 0, 0 });
		generateWall({ 0, -1, 0 });
		generateWall({ 0, 0, -1 });
		generateWall({ 1, 0, 0 });
		generateWall({ 0, 1, 0 }); 
		generateWall({ 0, 0, 1 }); 
		
	}

	void generateIndices() override
	{
		for (int i = 0; i < 24; i += 4)
		{
			indices.push_back(i); 
			indices.push_back(i+ 1); 
			indices.push_back(i+ 2); 
			indices.push_back(i+ 2); 
			indices.push_back(i+ 3); 
			indices.push_back(i); 
		}
	}

public : 
	Cube(glm::vec3 coordinates, glm::vec3 size, std::string texturePath,
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) :
		Primitive(coordinates, rotation, size, texturePath) 
	{
		generateVertices(); 
		generateIndices();
		init(); 
		shader = getShaderPtr(); 
	}
	static std::shared_ptr<Shader> getShaderPtr()
	{
		return ShaderProvider::instance().getShader("shCube.vert", "shCube.frag");
	}

	void scale(glm::vec3 amount)
	{
		size += amount;
	}


};
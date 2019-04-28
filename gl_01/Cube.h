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
#include "Texture.h"
#include <math.h>

class Cube : public Object
{
private:
	glm::vec3 coordinates; 
	glm::vec3 size; 
	glm::vec3 rotation; 
	GLuint VAO, VBO, EBO;
	GLuint texture0; 
	std::shared_ptr<Shader> shader; 
	const std::string texturePath; 
	std::vector <GLfloat> V;
	std::vector <GLuint> indices; 
	Texture texture; 

	void setUpBuffers()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);
		//static_cast<void*>(&vertices)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, V.size() * sizeof(V[0]), &V[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
		texture = Texture(texturePath); 
	}
	void insertVert(glm::vec3 v)
	{
		V.push_back(v.x);
		V.push_back(v.y); 
		V.push_back(v.z);
		int gray = (((V.size() - 1) / 5 % 4) >> 1) ^ (((V.size() - 1) / 5 % 4));
		
		V.push_back((GLfloat)(gray >> 1));
		V.push_back((GLfloat)(gray %2));
	}

	void generate()
	{
		glm::vec3 normal = glm::vec3(0, 0, 1);
		glm::vec3 direction = glm::vec3(-1, 0, 0);
		glm::vec3 firstPoint = glm::vec3(-1, -1, 1);

		insertVert(firstPoint);

		for (int i = 0; i < 3; i++)
		{
			firstPoint = cross(firstPoint, direction) + direction;
			insertVert(firstPoint);
		}
		for (int i = 0; i < 2; i++)
		{
			direction = glm::cross(direction, normal);
			for (int j = 0; j < 2; j++)
			{
				firstPoint = (cross(direction, firstPoint) *(float)pow(-1, i) + direction);
				insertVert(firstPoint);
			}
		}
		direction = glm::vec3(0, 0, 1);
		firstPoint = glm::vec3(1, -1, 1);
		for (int j = 0; j < 2; j++)
		{
			insertVert(glm::vec3(firstPoint.x, firstPoint.y, firstPoint.z * (GLfloat)(pow(-1, j))));
			for (int i = 0; i < 3; i++)
			{
				firstPoint = cross(firstPoint, direction) + direction;
				insertVert(glm::vec3(firstPoint.x, firstPoint.y, firstPoint.z * (GLfloat)(pow(-1, j))));
			}
		}


		int a = 0, b = 1, c = 2;
		for (int i = 0; i < 8; i++)
		{
			indices.push_back(a % 8);
			indices.push_back(b % 8);
			indices.push_back(c % 8);
			if (i % 2) { a += 2, c += 2; }
			else b += 2;
		}

		a = 8, b = 9, c = 10;
		for (int i = 0; i < 4; i++)
		{
			indices.push_back(a);
			indices.push_back(b);
			indices.push_back(c);
			if (!((i + 1) % 2)) { a += 4, c += 4; }
			b += 2;
		}
	}


public : 
	Cube(glm::vec3 coordinates, glm::vec3 size, std::string texturePath, glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f)) :
		texturePath(texturePath),
		size(size),
		rotation(rotation),
		coordinates(coordinates)
	{
		generate();
		shader = ShaderProvider::instance().getShader("shCube.vert", "shCube.frag"); 
		setUpBuffers(); 
	}

	~Cube()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	static std::shared_ptr<Shader> getShaderPtr()
	{
		return ShaderProvider::instance().getShader("shCube.vert", "shCube.frag");
	}
	
	void move(glm::vec3 displacement) override {
		this->coordinates += displacement;
	}
	void rotate(const glm::vec3& rotation) {
		this->rotation += rotation;
	}

	virtual void draw()
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, coordinates);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, size); 
		

		texture.useTexture(shader); 
		shader->setTransformMatrix("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}


};

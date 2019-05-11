#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "Object.h"
#include "ShaderProvider.h"
#include "Shader.h"
#include "Texture.h"
#include "SOIL.h"

/*
For pi constant
*/
#define _USE_MATH_DEFINES
#include <math.h>

/*
Number of segments which makes cylinder smooth enough
*/
#define DEFAULT_SEGMENTS_NUMBER 18

class Primitive : public Object
{
protected:

	glm::vec3 coordinates;
	glm::vec3 rotations;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::shared_ptr<Shader> shader;
	GLuint VAO, VBO, EBO;

	glm::mat4 model;
	Texture texture;
	const std::string texturePath;

	virtual void generateVertices() = 0;
	virtual void generateIndices() = 0;


	void setUpBuffers()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);
		//static_cast<void*>(&vertices)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	}

	/*
	Method setting the cylinder, called in constructor
	*/
	void init()
	{
		shader = ShaderProvider::instance().getShader("shCube.vert", "shCube.frag");
		generateVertices();
		generateIndices();
		setUpBuffers();
		texture = Texture(texturePath, false);
	}



public:
	Primitive(glm::vec3 coordinates, glm::vec3 rotations, std::string texturePath) 
	: coordinates(coordinates), rotations(rotations), texturePath(texturePath)
	{
	}

	virtual ~Primitive()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}



	virtual void draw()
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, coordinates);
		model = glm::rotate(model, glm::radians(rotations.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotations.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotations.z), glm::vec3(0.0f, 0.0f, 1.0f));
		texture.useTexture(shader);
		shader->setTransformMatrix("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	virtual void move(glm::vec3 displacement)
	{
		this->coordinates += displacement;
	}

	void rotate(const glm::vec3& rotations)
	{
		this->rotations += rotations;
	}
};

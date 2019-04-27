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
	glm::vec3 size; 
	GLfloat xRotation, yRotation, zRotation; 
	GLuint VAO, VBO, EBO;
	GLuint texture0; 
	std::shared_ptr<Shader> shader; 
	const std::string texturePath; 


	GLfloat vertices[40] = {
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		1.0f, 0.0f,
		-1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 
		-1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 
		1.0f, -1.0f, -1.0f,		0.0f, 1.0f,
		1.0f, -1.0f, 1.0f,		1.0f, 1.0f, 
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f, 
		1.0f, 1.0f, 1.0,		1.0f, 0.0f
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

	void setUpBuffers()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);
		//static_cast<void*>(&vertices)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 40 * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// prepare textures
		int width, height;
		unsigned char* image = SOIL_load_image(texturePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (image == nullptr)
			throw std::exception("Failed to load texture file");


		glGenTextures(1, &texture0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		// freeing unnecessary texture stuff
		SOIL_free_image_data(image);
		shader->use(); // don't forget to activate/use the shader before setting uniforms!
		// either set it manually like so:
		glUniform1i(glGetUniformLocation(shader->ID, "Texture0"), 0);
		// or set it via the texture class
		glBindTexture(GL_TEXTURE_2D, 0);

	}



public : 
	Cube(glm::vec3 coordinates, glm::vec3 size, std::string texturePath) :
		texturePath(texturePath),
		size(size),
		xRotation(0.0f), 
		yRotation(0.0f),
		zRotation(0.0f),
		coordinates(coordinates)
	{
		coordinates = glm::vec3(0.0f, 0.0f, 0.0f); 
		shader = ShaderProvider::instance().getShader("shCube.vert", "shCube.frag"); 
		setUpBuffers(); 
	}

	~Cube()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	
	virtual void move(glm::vec3 displacement)
	{

	}

	virtual void draw()
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, size); 
		model = glm::rotate(model, glm::radians(this->xRotation), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->yRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->zRotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, coordinates);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		shader->use();
		glUniform1i(glGetUniformLocation(shader->ID, "Texture0"), 0);
		shader->setTransformMatrix("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

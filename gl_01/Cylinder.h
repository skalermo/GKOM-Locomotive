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

class Cylinder : Object
{
private:

	/*
	Two planes - Top and Bottom, maybe will be changed later if there should be any planes between Top and Bottom
	*/
	const GLuint stacks = 2;

	/*
	Origin - center of the Bottom plane
	*/
	glm::vec3 coordinates;


	GLfloat height;
	GLfloat radius;

	/*
	Defines "smoothness" of a cylinder
	*/
	GLuint segments;

	/*
	Rotation variables, each one for each axis to rotate around
	*/
	GLfloat xRotation;
	GLfloat yRotation;
	GLfloat zRotation;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	std::shared_ptr<Shader> shader;
	GLuint VAO, VBO, EBO;
	Texture texture;
	const std::string texturePath;

	void generateVertices()
	{
		glm::vec3 centerCoords = glm::vec3(0.0f, 0.0f, 0.0f);
		GLfloat x, y, z = 0.0f, u, v;

		// Bottom plane
		vertices.push_back(centerCoords.x);
		vertices.push_back(centerCoords.y);
		vertices.push_back(centerCoords.z);
		vertices.push_back(0.25f);
		vertices.push_back(0.25f);
		for (size_t i = 0; i < segments; i++)
			{
				u = glm::cos((float)i / segments * 2 * (float)M_PI);
				v = glm::sin((float)i / segments * 2 * (float)M_PI);
				x = centerCoords.x + radius * u;
				y = centerCoords.y + radius * v;
				u = 0.25f + 0.25f * u;
				v = 0.25f + 0.25f * v;
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
				vertices.push_back(u);
				vertices.push_back(v);
		}

		// Top plane (just translated Bottom plane)
		for (size_t i = 0; i < segments + 1; i++)
		{
			vertices.push_back(vertices[5 * i]);
			vertices.push_back(vertices[5 * i + 1]);
			vertices.push_back(vertices[5 * i + 2] + height);
			vertices.push_back(vertices[5 * i + 3] + 0.5f);
			vertices.push_back(vertices[5 * i + 4]);
		}

		// Vertices of lateral surface
		for (size_t i = 0; i < stacks; i++)
		{
			GLfloat dh = (float)i / (float)(stacks - 1) * height;
			for (size_t j = 0; j < segments; j++)
			{
				vertices.push_back(vertices[5 * (j + 1)]);
				vertices.push_back(vertices[5 * (j + 1) + 1]);
				vertices.push_back(vertices[5 * (j + 1) + 2] + dh);
				vertices.push_back((float)j / (float)segments);
				vertices.push_back(0.5f + (float)i / (float)(2 * (stacks - 1)));
			}
			vertices.push_back(vertices[5]);
			vertices.push_back(vertices[6]);
			vertices.push_back(vertices[7] + dh);
			vertices.push_back(1.0f);
			vertices.push_back(0.5f + (float)i / (float)(2 * (stacks - 1)));
		}
	}

	void generateIndices()
	{

		/*
		Adding indices for triangles in Top and Bottom planes
		*/
		GLuint centerOfPlaneIndex;
		
		centerOfPlaneIndex = 0;
		indices.push_back(centerOfPlaneIndex);
		indices.push_back(centerOfPlaneIndex + segments);
		indices.push_back(centerOfPlaneIndex + 1);

		for (size_t j = 1; j <= segments - 1; j++)
		{
			indices.push_back(centerOfPlaneIndex);
			indices.push_back(centerOfPlaneIndex + j);
			indices.push_back(centerOfPlaneIndex + j + 1);
		}

		centerOfPlaneIndex = segments + 1;
		indices.push_back(centerOfPlaneIndex);
		indices.push_back(centerOfPlaneIndex + segments);
		indices.push_back(centerOfPlaneIndex + 1);

		for (size_t j = 1; j <= segments - 1; j++)
		{
			indices.push_back(centerOfPlaneIndex);
			indices.push_back(centerOfPlaneIndex + j);
			indices.push_back(centerOfPlaneIndex + j + 1);
		}
		

		/*
		Adding indices for triangles between planes
		*/
		GLuint lowerPlaneFirstIndex;
		GLuint higherPlaneFirstIndex;
		for (size_t i = 0; i < stacks - 1; i++)
		{
			lowerPlaneFirstIndex = (i + 2) * (segments + 1);
			higherPlaneFirstIndex = (i + 3) * (segments + 1);

			for (size_t j = 0; j < segments; j++)
			{
				indices.push_back(lowerPlaneFirstIndex + j);
				indices.push_back(higherPlaneFirstIndex + j);
				indices.push_back(higherPlaneFirstIndex + j + 1);

				indices.push_back(higherPlaneFirstIndex + j + 1);
				indices.push_back(lowerPlaneFirstIndex + j);
				indices.push_back(lowerPlaneFirstIndex + j + 1);
			}
		}
	}

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
		vertices.reserve((segments + 1) * (stacks + 2));
		indices.reserve(6 * segments * stacks);
		shader = ShaderProvider::instance().getShader("shCylinder.vert", "shCylinder.frag");
		generateVertices();
		generateIndices();
		setUpBuffers();
		texture = Texture(texturePath, true);
	}
	
	

public:
	// template solution
	static std::shared_ptr<Shader> getShaderPtr()
	{
		return ShaderProvider::instance().getShader("shCylinder.vert", "shCylinder.frag");
	}

	/*
	Short version constructor
	*/
	Cylinder(glm::vec3 coordinates, GLfloat height, GLfloat radius, std::string texturePath) :
		coordinates(coordinates),
		height(height),
		radius(radius),
		segments(DEFAULT_SEGMENTS_NUMBER),
		xRotation(0.0f),
		yRotation(0.0f),
		zRotation(0.0f),
		texturePath(texturePath)
	{
		init();
	}

	/*
	Long version constructor
	*/
	Cylinder(glm::vec3 coordinates, GLfloat height, GLfloat radius, GLuint segments, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation, std::string texturePath) :
		coordinates(coordinates),
		height(height),
		radius(radius),
		segments(segments),
		xRotation(xRotation),
		yRotation(yRotation),
		zRotation(zRotation),
		texturePath(texturePath)
	{
		init();
	}

	~Cylinder() 
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}



	virtual void draw()
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(this->xRotation), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->yRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(this->zRotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, coordinates);

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

	void rotate(GLfloat xRotation, GLfloat yRotation, GLfloat zRotation)
	{
		this->xRotation += xRotation;
		this->yRotation += yRotation;
		this->zRotation += zRotation;
	}

	/*
	Debugging xd
	*/
	void print()
	{
		std::cout << "Vertices" << std::endl;
		for (size_t i = 0; i < vertices.size()/5; i++)
		{
			std::cout << i << ") "<< vertices[5 * i] << ' ';
			std::cout << vertices[5 * i + 1] << ' ';
			std::cout << vertices[5 * i + 2] << ' ';
			std::cout << vertices[5 * i + 3] << ' ';
			std::cout << vertices[5 * i + 4] << std::endl;
		}
		std::cout << "Indices" << std::endl;
		for (size_t i = 0; i < indices.size() / 3; i++)
		{
			std::cout << 3 * i << ") " << indices[3 * i] << ' ';
			std::cout << 3 * i + 1 << ") " << indices[3 * i + 1] << ' ';
			std::cout << 3 * i + 2 << ") " << indices[3 * i + 2] << std::endl;
		}
	}
};
#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Object.h"
#include "ShaderProvider.h"
#include "Shader.h"

/*
For pi constant
*/
#define _USE_MATH_DEFINES
#include <math.h>

#define NULL_VECTOR glm::vec3(0.0f, 0.0f, 0.0f)

/*
Number of segments which makes cylinder smooth enough
*/
#define DEFAULT_SEGMENTS_NUMBER 18

class Cylinder : Object
{
private:


	/*
	Two planes - Top and Bottom, maybe will be changed later if there will be any planes between Top and Bottom
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


	void generateVertices()
	{
		glm::vec3 centerCoords = coordinates;
		
		for (size_t i = 0; i < stacks; i++)
		{
			centerCoords.z = (float) (height * i / (stacks - 1));

			vertices.push_back(centerCoords.x);
			vertices.push_back(centerCoords.y);
			vertices.push_back(centerCoords.z);
				
			float z = centerCoords.z, x, y;
			for (size_t j = 0; j < segments; j++)
			{
				x = centerCoords.x + radius * glm::cos((float)j / segments * 2 * (float)M_PI);
				y = centerCoords.y + radius * glm::sin((float)j / segments * 2 * (float)M_PI);
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}
		}
	}

	void generateIndices()
	{

		/*
		Adding indices for triangles in Top and Bottom planes (and also between them if stacks > 2)
		*/
		GLuint centerOfPlaneIndex;
		for (size_t i = 0; i < stacks; i++)
		{
			centerOfPlaneIndex = i * (segments + 1);
			indices.push_back(centerOfPlaneIndex);
			indices.push_back(centerOfPlaneIndex + segments);
			indices.push_back(centerOfPlaneIndex + 1);

			for (size_t j = 1; j <= segments - 1; j++)
			{
				indices.push_back(centerOfPlaneIndex);
				indices.push_back(centerOfPlaneIndex + j);
				indices.push_back(centerOfPlaneIndex + j + 1);
			}
		}

		/*
		Adding indices for triangles between planes
		*/
		GLuint centerIndexLowerPlane;
		GLuint centerIndexHigherPlane;
		for (size_t i = 0; i < stacks - 1; i++)
		{
			centerIndexLowerPlane = i * (segments + 1);
			centerIndexHigherPlane = (i + 1) * (segments + 1);

			indices.push_back(centerIndexLowerPlane + segments);
			indices.push_back(centerIndexHigherPlane + segments);
			indices.push_back(centerIndexHigherPlane + 1);
			indices.push_back(centerIndexHigherPlane + 1);
			indices.push_back(centerIndexLowerPlane + segments);
			indices.push_back(centerIndexLowerPlane + 1);
			for (size_t j = 1; j <= segments - 1; j++)
			{
				indices.push_back(centerIndexLowerPlane + j);
				indices.push_back(centerIndexHigherPlane + j);
				indices.push_back(centerIndexHigherPlane + j + 1);
				indices.push_back(centerIndexHigherPlane + j + 1);
				indices.push_back(centerIndexLowerPlane + j);
				indices.push_back(centerIndexLowerPlane + j + 1);
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

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	
	}

	/*
	Method setting the cylinder, called in constructor
	*/
	void init()
	{
		vertices.reserve(2 * (segments + 1));
		indices.reserve(4 * segments);
		shader = ShaderProvider::instance().getShader("cylinderSh.vert", "cylinderSh.frag");
		generateVertices();
		generateIndices();
		setUpBuffers();
	}
	
	

public:
	// template solution, shouldnt be in pushed version
	static std::shared_ptr<Shader> getShaderPtr()
	{
		return ShaderProvider::instance().getShader("cylinderSh.vert", "cylinderSh.frag");
	}

	/*
	Default constructor with fixed parameters
	*/
	Cylinder() :
		coordinates(NULL_VECTOR),
		height(1.0),
		radius(1.0),
		segments(DEFAULT_SEGMENTS_NUMBER),
		xRotation(0.0f),
		yRotation(0.0f),
		zRotation(0.0f)
	{
		init();
	}

	/*
	Short version constructor
	*/
	Cylinder(glm::vec3 coordinates, GLfloat height, GLfloat radius) :
		coordinates(coordinates),
		height(height),
		radius(radius),
		segments(DEFAULT_SEGMENTS_NUMBER),
		xRotation(0.0f),
		yRotation(0.0f),
		zRotation(0.0f)
	{
		init();
	}

	/*
	Long version constructor
	*/
	Cylinder(glm::vec3 coordinates, GLfloat height, GLfloat radius, GLuint segments, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation):
		coordinates(coordinates),
		height(height),
		radius(radius),
		segments(segments),
		xRotation(xRotation),
		yRotation(yRotation),
		zRotation(zRotation)
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
		shader->use();
		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view = glm::mat4(1.0f);;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		shader->setTransformMatrix("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, coordinates);
		model = glm::rotate(model, glm::radians(xRotation), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(yRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(zRotation), glm::vec3(0.0f, 0.0f, 1.0f));
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
		// todo: Add translation move to 0,0,0 first then rotate then translation back
		this->xRotation = xRotation;
		this->yRotation = yRotation;
		this->zRotation = zRotation;

	}

	/*
	Debugging xd
	*/
	void print()
	{
		std::cout << "Vertices" << std::endl;
		for (size_t i = 0; i < vertices.size()/3; i++)
		{
			std::cout << vertices[3 * i] << ' ';
			std::cout << vertices[3 * i + 1] << ' ';
			std::cout << vertices[3 * i + 2] << std::endl;
		}
		std::cout << "Indices" << std::endl;
		for (size_t i = 0; i < indices.size() / 3; i++)
		{
			std::cout << 3 * i << ") " << indices[3 * i] << "\t";
			std::cout << 3 * i + 1 << ") " << indices[3 * i + 1] << "\t";
			std::cout << 3 * i + 2 << ") " << indices[3 * i + 2] << std::endl;
		}
	}
};
#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Object.h"

/*
For pi constant
*/
#define _USE_MATH_DEFINES
#include <math.h>



/*
Number of segments which makes cylinder smooth enough
*/
#define DEFAULT_SEGMENTS_NUMBER 6

/*
Unit vector
*/
#define UNIT_VECTOR glm::vec3(1.0f, 0.0f, 0.0f)

class Cylinder : Object
{
private:


	/*
	Two planes - Top and Bottom, maybe won't be needed later if there will be another planes between them
	*/
	const GLuint stacks = 2;

	/*
	Arbitrary point - center of the Bottom plane
	*/
	glm::vec3 coordinates;


	GLfloat height;
	GLfloat radius;

	/*
	Defines "smoothness" of a cylinder
	*/
	GLuint segments;

	/*
	Vector orthogonal to cylinder's planes. Allows to rotate cylinder
	*/
	glm::vec3 normal;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;
	GLuint VAO, VBO, EBO;

	void generateVertices()
	{
		glm::vec3 centerCoords;
		if (normal == UNIT_VECTOR)
		{
			for (size_t i = 0; i < stacks; i++)
			{
				centerCoords = coordinates + UNIT_VECTOR * (float) (height * i / (stacks - 1));

				vertices.push_back(centerCoords.x);
				vertices.push_back(centerCoords.y);
				vertices.push_back(centerCoords.z);
				
				float z = centerCoords.z, y, x;
				for (size_t j = 0; j < segments; j++)
				{

					x = centerCoords.y + radius * glm::cos((float)j / segments * 2 * (float)M_PI);
					y = centerCoords.y + radius * glm::sin((float)j / segments * 2 * (float)M_PI);
					vertices.push_back(x);
					vertices.push_back(y);
					vertices.push_back(z);
				}
			}
		}
		/*
		vertices = { 0.5f, 0.5f, 0.0f,  // Верхний правый угол
			0.5f, -0.5f, 0.0f,  // Нижний правый угол
			-0.5f, -0.5f, 0.0f,  // Нижний левый угол
			-0.5f, 0.5f, 0.0f };  // Верхний левый угол*/
	}

	void generateIndices()
	{

		/*
		Adding indices for triangles in Top and Bottom planes
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
		/*
		indices = { 0, 1, 3,   // Первый треугольник
			1, 2, 3 };    // Второй треугольник*/
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

		generateVertices();
		generateIndices();
		setUpBuffers();
	}

public:

	Cylinder() :
		coordinates(UNIT_VECTOR),
		height(10),
		radius(5),
		segments(DEFAULT_SEGMENTS_NUMBER),
		normal(UNIT_VECTOR) 
	{
		init();
	}

	Cylinder(glm::vec3 coords, GLfloat h, GLfloat r) :
		coordinates(coords),
		height(h),
		radius(r),
		segments(DEFAULT_SEGMENTS_NUMBER),
		normal(UNIT_VECTOR)
	{
		init();
	}

	Cylinder(glm::vec3 coords, GLfloat h, GLfloat r, GLuint s, glm::vec3 norm):
		coordinates(coords),
		height(h),
		radius(r),
		segments(s),
		normal(norm)
	{
		//todo
	}

	~Cylinder() 
	{
		clean();
	}

	virtual void draw()
	{
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	
	virtual void move(glm::vec3 displacement)
	{
		this->coordinates += displacement;
		//todo
	}

	void print()
	{
		for (size_t i = 0; i < vertices.size()/3; i++)
		{
			std::cout << 3 * i << ") " << vertices[3 * i] << "\t";
			std::cout << 3 * i + 1 << ") " << vertices[3 * i + 1] << "\t";
			std::cout << 3*i+2 << ") " << vertices[3 * i + 2] << std::endl;
		}
		for (size_t i = 0; i < indices.size() / 3; i++)
		{
			std::cout << 3 * i << ") " << indices[3 * i] << "\t";
			std::cout << 3 * i + 1 << ") " << indices[3 * i + 1] << "\t";
			std::cout << 3 * i + 2 << ") " << indices[3 * i + 2] << std::endl;
		}
	}

	void clean()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
};

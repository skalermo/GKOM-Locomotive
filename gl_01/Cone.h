#pragma once
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
#include "Primitive.h"

/*
Number of segments which makes cylinder smooth enough
*/
#define DEFAULT_SEGMENTS_NUMBER 18

class Cone : public Primitive
{
private:

	/*
	Two planes - Top and Bottom, maybe will be changed later if there should be any planes between Top and Bottom
	*/
	const GLuint stacks = 2;

	GLfloat height;
	GLfloat radius;

	/*
	Defines "smoothness" of a cylinder
	*/
	GLuint segments;

	void generateVertices() override
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

		vertices.push_back(centerCoords.x);
		vertices.push_back(centerCoords.y);
		vertices.push_back(centerCoords.z + height);
		vertices.push_back(0.25f);
		vertices.push_back(0.25f);
	}

	void generateIndices() override
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



		int top = vertices.size()  / 5 - 1;
		for (size_t i = 1; i <= segments ; i++)
		{
			int bottomV = i ;
			int bottomV2 = i + 1;

			indices.push_back(bottomV);
			indices.push_back(bottomV2);
			indices.push_back(top);
		}

		indices.push_back(1);
		indices.push_back(top -1);
		indices.push_back(top);
	}


public:

	/*
	Short version constructor
	*/
	Cone(glm::vec3 coordinates, GLfloat height, GLfloat radius, std::string texturePath) :
		Cone(coordinates, height, radius, DEFAULT_SEGMENTS_NUMBER, glm::vec3(0.0f), texturePath) {}

	/*
	Long version constructor
	*/
	Cone(glm::vec3 coordinates, GLfloat height, GLfloat radius, GLuint segments, glm::vec3 rotations, std::string texturePath) :
		Primitive(coordinates, rotations, texturePath),
		height(height),
		radius(radius),
		segments(segments)
	{
		init();
	}
};

#pragma once

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include <string>
#include <cmath>
#include "Primitive.h"

#define _USE_MATH_DEFINES

/*
Number of segments which makes cylinder smooth enough
*/
#define DEFAULT_SEGMENTS_NUMBER 18

class Cone : public Primitive
{
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
		const auto centerCoords = glm::vec3(0.0f, 0.0f, 0.0f);
		const auto z = 0.0f;

		// Bottom plane
		vertices.push_back(centerCoords.x);
		vertices.push_back(centerCoords.y);
		vertices.push_back(centerCoords.z);
		vertices.push_back(1);//normals
		vertices.push_back(1);//normals
		vertices.push_back(1);//normals
		vertices.push_back(0.25f);
		vertices.push_back(0.25f);
		for (size_t i = 0; i < segments; i++)
		{
			auto u = glm::cos(static_cast<float>(i) / segments * 2 * static_cast<float>(M_PI));
			auto v = glm::sin(static_cast<float>(i) / segments * 2 * static_cast<float>(M_PI));
			auto x = centerCoords.x + radius * u;
			auto y = centerCoords.y + radius * v;
			u = 0.25f + 0.25f * u;
			v = 0.25f + 0.25f * v;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
			vertices.push_back(1);//normals
			vertices.push_back(1);//normals
			vertices.push_back(1);//normals
			vertices.push_back(u);
			vertices.push_back(v);
		}

		vertices.push_back(centerCoords.x);
		vertices.push_back(centerCoords.y);
		vertices.push_back(centerCoords.z + height);
		vertices.push_back(1);//normals
		vertices.push_back(1);//normals
		vertices.push_back(1);//normals
		vertices.push_back(0.25f);
		vertices.push_back(0.25f);
	}

	void generateIndices() override
	{
		const GLuint centerOfPlaneIndex = 0;
		indices.push_back(centerOfPlaneIndex);
		indices.push_back(centerOfPlaneIndex + segments);
		indices.push_back(centerOfPlaneIndex + 1);

		for (size_t j = 1; j <= segments - 1; j++)
		{
			indices.push_back(centerOfPlaneIndex);
			indices.push_back(centerOfPlaneIndex + j);
			indices.push_back(centerOfPlaneIndex + j + 1);
		}


		const int top = vertices.size() / 8 - 1;
		for (size_t i = 1; i <= segments; i++)
		{
			const int bottomV = i;
			const int bottomV2 = i + 1;

			indices.push_back(bottomV);
			indices.push_back(bottomV2);
			indices.push_back(top);
		}

		indices.push_back(1);
		indices.push_back(top - 1);
		indices.push_back(top);
	}


public:

	Cone(glm::vec3 coordinates, GLfloat height, GLfloat radius, std::string texturePath) :
		Cone(coordinates, height, radius, DEFAULT_SEGMENTS_NUMBER, glm::vec3(0.0f), texturePath) {}

	Cone(glm::vec3 coordinates, GLfloat height, GLfloat radius, GLuint segments, glm::vec3 rotations, std::string texturePath) :
		Primitive(coordinates, rotations, { 1, 1, 1 }, texturePath),
		height(height),
		radius(radius),
		segments(segments)
	{
		init();
	}
};

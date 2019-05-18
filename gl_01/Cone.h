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

	void pushVertex(glm::vec3 vertex)
	{
		vertices.push_back(vertex.x);
		vertices.push_back(vertex.y);
		vertices.push_back(vertex.z);
	}

	void pushTriangle(glm::uvec3 triangle)
	{
		indices.push_back(triangle.x);
		indices.push_back(triangle.y);
		indices.push_back(triangle.z);
	}

	void pushTexture(float u, float v)
	{
		vertices.push_back(u);
		vertices.push_back(v);
	}

	glm::vec3 vertexFromIndex(unsigned int index)
	{
		return glm::vec3{ vertices[index * 8], vertices[index * 8 + 1], vertices[index * 8 + 2] };
	}

	void changeNormal(unsigned int index, glm::vec3 normal)
	{
		vertices[index * 8 + 3] = normal.x;
		vertices[index * 8 + 4] = normal.y;
		vertices[index * 8 + 5] = normal.z;
	}
	glm::vec3 getNormal(unsigned int index)
	{
		return glm::vec3{ vertices[index * 8 + 3],vertices[index * 8 + 4] , vertices[index * 8 + 5] };
	}

	glm::vec3 calculateNormal(glm::uvec3 triangle)
	{
		const auto v1 = vertexFromIndex(triangle.x);
		const auto v2 = vertexFromIndex(triangle.y);
		const auto v3 = vertexFromIndex(triangle.z);
		const auto normal = glm::cross(v2 - v1, v3 - v1);
		return normal;
	}

	void generateVertices() override
	{
		const auto centerCoords = glm::vec3(0.0f, 0.0f, 0.0f);
		const auto centerNormal = glm::vec3(0, 0, -1);
		const auto z = 0.0f;

		// Bottom plane
		pushVertex(centerCoords);
		pushVertex(centerNormal);
		pushTexture(0.25f, 0.25f);
		auto basis = true;
		for (int i = 0; i < 2; ++i)
		{
			for (size_t i = 0; i < segments; i++)
			{
				auto u = glm::cos(static_cast<float>(i) / segments * 2 * static_cast<float>(M_PI));
				auto v = glm::sin(static_cast<float>(i) / segments * 2 * static_cast<float>(M_PI));
				auto x = centerCoords.x + radius * u;
				auto y = centerCoords.y + radius * v;

				u = 0.25f + 0.25f * u;
				v = 0.25f + 0.25f * v;

				pushVertex({ x,y,z });
				if (basis) pushVertex(centerNormal);
				else pushVertex({ 1,1,1 });//dummy normal
				pushTexture(u, v);
			}
			basis = false;
		}

		//TIP of the cone
		for (size_t i = 0; i < segments; ++i)
		{
			pushVertex(centerCoords + glm::vec3{ 0, 0, height });
			pushVertex({ 1,1,1 });//dummy normal
			pushTexture(0.25f, 0.25f);
		}
	}

	void generateIndices() override
	{
		const GLuint centerOfPlaneIndex = 0;
		pushTriangle({ centerOfPlaneIndex, centerOfPlaneIndex + segments, centerOfPlaneIndex + 1 });

		//Basis
		for (size_t j = 1; j <= segments - 1; j++)
		{
			pushTriangle({ centerOfPlaneIndex, centerOfPlaneIndex + j, centerOfPlaneIndex + j + 1 });
		}


		for (size_t i = 1 + segments; i <= segments * 2; i++)
		{
			const int top = i + segments;
			const auto triangle = glm::uvec3{ i, i + 1, top };
			const auto normal = (calculateNormal(triangle) + getNormal(i - 1)) / glm::vec3(2);
			pushTriangle(triangle);
			changeNormal(i, normal);
			changeNormal(top, normal);
		}

		const int top = vertices.size() / 8 - 1;
		const auto triangle = glm::uvec3{ segments + 1, segments * 2, top };
		pushTriangle(triangle);
	}


public:

	Cone(glm::vec3 coordinates, GLfloat height, GLfloat radius, std::string texturePath) :
		Cone(coordinates, height, radius, DEFAULT_SEGMENTS_NUMBER, glm::vec3(0.0f), texturePath) {}

	Cone(glm::vec3 coordinates, GLfloat height, GLfloat radius, GLuint segments, glm::vec3 rotations, std::string texturePath) :
		Primitive(coordinates, rotations, texturePath),
		height(height),
		radius(radius),
		segments(segments)
	{
		init();
	}
};

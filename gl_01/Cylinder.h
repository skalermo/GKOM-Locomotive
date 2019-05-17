#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "ShaderProvider.h"
#include "Primitive.h"

/*
For pi constant
*/
#define _USE_MATH_DEFINES
#include <math.h>

class Cylinder : public Primitive
{
private:

	/*
	Two planes - Top and Bottom, maybe will be changed later if there should be any planes between Top and Bottom
	*/
	const GLuint stacks = 2;

	/*
	Number of segments which makes cylinder smooth enough
	*/
	static const unsigned int DEFAULT_SEGMENTS_NUMBER = 18;

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

		// vertex position
		vertices.push_back(centerCoords.x);
		vertices.push_back(centerCoords.y);
		vertices.push_back(centerCoords.z);

		// normal vector
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
		vertices.push_back(-1.0f);

		// uv coordinates
		vertices.push_back(0.25f);
		vertices.push_back(0.25f);
		for (size_t i = 0; i < segments; i++)
		{
			u = glm::cos(static_cast<float>(i) / segments * 2 * static_cast<float>(M_PI));
			v = glm::sin(static_cast<float>(i) / segments * 2 * static_cast<float>(M_PI));
			x = centerCoords.x + radius * u;
			y = centerCoords.y + radius * v;

			u = 0.25f + 0.25f * u;
			v = 0.25f + 0.25f * v;

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(-1.0f);

			vertices.push_back(u);
			vertices.push_back(v);
		}

		// Top plane (just translated Bottom plane)
		for (size_t i = 0; i < segments + 1; i++)
		{
			vertices.push_back(vertices[8 * i]);
			vertices.push_back(vertices[8 * i + 1]);
			vertices.push_back(vertices[8 * i + 2] + height);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(vertices[8 * i + 6] + 0.5f);
			vertices.push_back(vertices[8 * i + 7]);
		}

		// Vertices of lateral surface
		for (size_t i = 0; i < stacks; i++)
		{
			float dh = static_cast<GLfloat>(i) / static_cast<GLfloat>(stacks - 1) * height;
			for (size_t j = 0; j < segments; j++)
			{
				vertices.push_back(vertices[8 * (j + 1)]);
				vertices.push_back(vertices[8 * (j + 1) + 1]);
				vertices.push_back(vertices[8 * (j + 1) + 2] + dh);

				vertices.push_back(vertices[8 * (j + 1)] - vertices[0]);
				vertices.push_back(vertices[8 * (j + 1) + 1] - vertices[1]);
				vertices.push_back(0.0f);

				vertices.push_back(static_cast<GLfloat>(j) / static_cast<GLfloat>(segments));
				vertices.push_back(0.5f + static_cast<GLfloat>(i) / static_cast<GLfloat>(2 * (stacks - 1)));
			}
			vertices.push_back(vertices[8]);
			vertices.push_back(vertices[9]);
			vertices.push_back(vertices[10] + dh);

			vertices.push_back(vertices[8] - vertices[0]);
			vertices.push_back(vertices[8 + 1] - vertices[1]);
			vertices.push_back(0.0f);

			vertices.push_back(1.0f);
			vertices.push_back(0.5f + static_cast<GLfloat>(i) / static_cast<GLfloat>(2 * (stacks - 1)));
		}
		
	}

	void generateIndices() override
	{

		/*
		Adding indices for triangles in Top and Bottom planes
		*/
		unsigned int centerOfPlaneIndex;

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

public:
	/*
	Short version constructor
	*/
	Cylinder(glm::vec3 coordinates, GLfloat height, GLfloat radius, std::string texturePath) :
        Cylinder(coordinates, height, radius, DEFAULT_SEGMENTS_NUMBER, glm::vec3(0.0f), texturePath) {}
        
	/*
	Long version constructor
	*/
	Cylinder(glm::vec3 coordinates, GLfloat height, GLfloat radius, GLuint segments, glm::vec3 rotations, std::string texturePath)
	: Primitive(coordinates, rotations, texturePath),
		height(height),
		radius(radius),
		segments(segments)
	{
		shader = getShaderPtr();
		init();
	}

	static std::shared_ptr<Shader> getShaderPtr() {
		return ShaderProvider::instance().getShader("shCylinder.vert", "shCylinder.frag");
	}
};
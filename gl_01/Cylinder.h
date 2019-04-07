#pragma once

#include <GL/glew.h>
#include "Object.h"
#include "Composite.h"

/*
Number of segments which makes cylinder smooth enough
*/
#define DEFAULT_SEGMENTS_NUMBER 25

/*
Unit vector
*/
#define DEFAULT_VECTOR glm::vec3(1.0f, 0.0f, 0.0f)

class Cylinder : Object
{
private:
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
	Vector pointing at the opposite Top plane. Allows to rotate cylinder
	*/
	glm::vec3 direction;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	void generateVertices(std::vector<GLfloat> vert, GLuint segm)
	{
		//todo
	}

	void generateIndices(std::vector<GLuint> ind, GLuint segm)
	{
		/*
		Number of planes
		*/
		const GLuint stacks = 2;

		/*
		Adding indices for triangles in Top and Bottom planes
		*/
		GLuint centerOfPlaneIndex;
		for (size_t i = 0; i < stacks; i++)
		{
			centerOfPlaneIndex = i * (segm + 1);
			ind.push_back(centerOfPlaneIndex);
			ind.push_back(centerOfPlaneIndex + segm);
			ind.push_back(centerOfPlaneIndex + 1);

			for (size_t j = 1; j <= segm - 1; j++)
			{
				ind.push_back(centerOfPlaneIndex);
				ind.push_back(centerOfPlaneIndex + j);
				ind.push_back(centerOfPlaneIndex + j + 1);
			}
		}

		/*
		Adding indices for triangles between planes
		*/
		GLuint centerIndexLowerPlane;
		GLuint centerIndexHigherPlane;
		for (size_t i = 0; i < stacks - 1; i++)
		{
			centerIndexLowerPlane = i * (segm + 1);
			centerIndexHigherPlane = (i + 1) * (segm + 1);

			ind.push_back(centerIndexLowerPlane + segm);
			ind.push_back(centerIndexHigherPlane + segm);
			ind.push_back(centerIndexHigherPlane + 1);
			ind.push_back(centerIndexHigherPlane + 1);
			ind.push_back(centerIndexLowerPlane + segm);
			ind.push_back(centerIndexLowerPlane + 1);
			for (size_t j = 1; j <= 2 * (segm - 1); j++)
			{
				ind.push_back(centerIndexLowerPlane + j);
				ind.push_back(centerIndexHigherPlane + j);
				ind.push_back(centerIndexHigherPlane + j + 1);
				ind.push_back(centerIndexHigherPlane + j + 1);
				ind.push_back(centerIndexLowerPlane + j);
				ind.push_back(centerIndexLowerPlane + j + 1);
			}
		}
	}

	/*
	Method setting the cylinder, called in constructor
	*/
	void init()
	{
		vertices.reserve(2 * (segments + 1));
		indices.reserve(4 * segments);

		generateVertices(vertices, segments);
		generateIndices(indices, segments);
	}

public:

	Cylinder() :
		coordinates(DEFAULT_VECTOR),
		height(10),
		radius(5),
		segments(DEFAULT_SEGMENTS_NUMBER),
		direction(DEFAULT_VECTOR) 
	{
		//init();
		//todo
	}

	Cylinder(glm::vec3 coords, GLfloat h, GLfloat r) :
		coordinates(coords),
		height(h),
		radius(r),
		segments(DEFAULT_SEGMENTS_NUMBER),
		direction(DEFAULT_VECTOR)
	{
		//todo
	}

	Cylinder(glm::vec3 coords, GLfloat h, GLfloat r, GLuint s, glm::vec3 dir):
		coordinates(coords),
		height(h),
		radius(r),
		segments(s),
		direction(dir)
	{
		//todo
	}

	~Cylinder() {}

	virtual void draw()
	{
		//todo
	}
	
	virtual void move(glm::vec3 displacement)
	{
		this->coordinates += displacement;
		//todo
	}
};
